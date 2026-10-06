// Copyright (c) 2022 ChenJun
// Licensed under the Apache-2.0 License.

#include "rm_serial_driver/rm_serial_driver.hpp"

#include <tf2/LinearMath/Quaternion.h>

#include <cmath>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <rclcpp/logging.hpp>
#include <rclcpp/qos.hpp>
#include <rclcpp/utilities.hpp>
#include "rm_serial_driver/crc.hpp"
#include "rm_serial_driver/default_receive_protocol.hpp"
#include "rm_serial_driver/packet.hpp"
#include <serial_driver/serial_driver.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

namespace rm_serial_driver
{

RMSerialDriver::RMSerialDriver(const rclcpp::NodeOptions & options)
: Node("rm_serial_driver", options),
  owned_ctx_{new IoContext(2)},
  serial_driver_{new drivers::serial_driver::SerialDriver(*owned_ctx_)}
{
  RCLCPP_INFO(get_logger(), "Start RMSerialDriver!");

  getParams();

  // for debug
  is_open_fire_control = this->declare_parameter("is_open_fire_control", true);
  // TF broadcaster
  timestamp_offset_ = this->declare_parameter("timestamp_offset", 0.0);
  yaw_to_pitch_distance_ = this->declare_parameter("yaw_to_pitch_distance", 0.0);
  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
  // Create Publisher
  task_pub_ = this->create_publisher<std_msgs::msg::String>("task_mode", 10);
  latency_pub_ = this->create_publisher<std_msgs::msg::Float64>("latency", 10);
  bullet_speed_pub_ = this->create_publisher<std_msgs::msg::Float32>("bullet_speed", 10);
  record_controller_pub_ = this->create_publisher<std_msgs::msg::String>("record_controller", 10);
  is_play_pub_ = this->create_publisher<std_msgs::msg::Bool>("is_play", 10);
  // marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("/aiming_point", 10);
  aim_time_info_pub_ =
    this->create_publisher<rm_interfaces::msg::TimeInfo>("time_info/aim", 10);

  // Detect parameter clients
  armor_detector_param_client_ =
    std::make_shared<rclcpp::AsyncParametersClient>(this, "armor_detector");
  buff_detector_param_client_ =
    std::make_shared<rclcpp::AsyncParametersClient>(this, "buff_detector");

  // Tracker reset service client
  reset_tracker_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/reset");

  // Target change service cilent
  change_target_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/change");

  // Configure adapters and handlers before the receive thread can use them.
  initReceivePipeline();

  try {
    serial_driver_->init_port(device_name_, *device_config_);
    if (!serial_driver_->port()->is_open()) {
      serial_driver_->port()->open();
      receive_thread_ = std::thread(&RMSerialDriver::receiveData, this);
    }
  } catch (const std::exception & ex) {
    RCLCPP_ERROR(
      get_logger(), "Error creating serial port: %s - %s", device_name_.c_str(), ex.what());
    throw ex;
  }

  // Create Subscription
  aim_sub_ = this->create_subscription<rm_interfaces::msg::GimbalCmd>(
    "armor_planner/cmd_gimbal", rclcpp::SensorDataQoS(),
    std::bind(&RMSerialDriver::sendArmorData, this, std::placeholders::_1));
  buff_sub_ = this->create_subscription<rm_interfaces::msg::GimbalCmd>(
    "buff_tracker/cmd_gimbal", rclcpp::SensorDataQoS(),
    std::bind(&RMSerialDriver::sendBuffData, this, std::placeholders::_1));
  heartbeat_ = rm_tools::HeartBeatPublisher::create(this);
}
RMSerialDriver::~RMSerialDriver()
{
  if (receive_thread_.joinable()) {
    receive_thread_.join();
  }

  if (serial_driver_->port()->is_open()) {
    serial_driver_->port()->close();
  }

  if (owned_ctx_) {
    owned_ctx_->waitForExit();
  }
}

void RMSerialDriver::receiveData()
{
  std::vector<uint8_t> header(1);
  std::vector<uint8_t> data;
  data.reserve(receive_pipeline_->frameSize());

  while (rclcpp::ok()) {
    try {
      serial_driver_->port()->receive(header);
      if (header[0] != receive_pipeline_->headerByte()) {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 20, "Invalid header: %02X", header[0]);
        continue;
      }

      // Keep the original fixed-size serial read; the pipeline owns decoding.
      data.resize(receive_pipeline_->frameSize() - 1);
      serial_driver_->port()->receive(data);
      data.insert(data.begin(), header[0]);

      const DecodeStatus result = receive_pipeline_->processFrame(data);
      if (result == DecodeStatus::InvalidChecksum) {
        RCLCPP_ERROR(get_logger(), "CRC error!");
      } else if (result != DecodeStatus::Ok) {
        RCLCPP_WARN(get_logger(), "Rejected receive frame: %d", static_cast<int>(result));
      }
    } catch (const std::exception & ex) {
      RCLCPP_ERROR_THROTTLE(
        get_logger(), *get_clock(), 20, "Error while receiving data: %s", ex.what());
      reopenPort();
    }
  }
}

void RMSerialDriver::initReceivePipeline()
{
  receive_pipeline_ =
    std::make_unique<ReceivePipeline>(std::make_unique<DefaultReceiveProtocol>());
  receive_pipeline_->addHandler(std::make_unique<ControlHandler>(*this));
  receive_pipeline_->addHandler(std::make_unique<StatusHandler>(*this));
  receive_pipeline_->addHandler(std::make_unique<GimbalHandler>(*this));
}

void RMSerialDriver::syncDetectColor(uint8_t color)
{
  // Preserve the original retry/version logic for both detector nodes.
  if (!has_desired_detect_color_.load() || color != desired_detect_color_.load()) {
    desired_detect_color_.store(color);
    has_desired_detect_color_.store(true);
    detect_color_param_version_.fetch_add(1);
    armor_detect_color_synced_.store(false);
    buff_detect_color_synced_.store(false);
  }
  if (!armor_detect_color_synced_.load() || !buff_detect_color_synced_.load()) {
    setParam(rclcpp::Parameter("detect_color", static_cast<int>(color)));
  }
}

void RMSerialDriver::publishStatus(const StatusData & status)
{
  std_msgs::msg::String task;
  task.data = taskModeName(status.task_mode);
  task_pub_->publish(task);

  std_msgs::msg::Float32 bullet_speed;
  bullet_speed.data = status.bullet_speed;
  bullet_speed_pub_->publish(bullet_speed);

  std_msgs::msg::String record_controller;
  record_controller.data = status.is_play ? "start" : "stop";
  record_controller_pub_->publish(record_controller);

  std_msgs::msg::Bool is_play;
  is_play.data = status.is_play;
  is_play_pub_->publish(is_play);
}

void RMSerialDriver::publishGimbal(const GimbalData & gimbal)
{
  timestamp_offset_ = this->get_parameter("timestamp_offset").as_double();
  const auto stamp = this->now() - rclcpp::Duration::from_seconds(timestamp_offset_);
  const double yaw_to_pitch_distance =
    this->get_parameter("yaw_to_pitch_distance").as_double();

  geometry_msgs::msg::TransformStamped yaw_t;
  yaw_t.header.stamp = stamp;
  yaw_t.header.frame_id = "odom";
  yaw_t.child_frame_id = "yaw_link";
  yaw_t.transform.translation.x = 0.0;
  yaw_t.transform.translation.y = 0.0;
  yaw_t.transform.translation.z = 0.0;
  tf2::Quaternion q_yaw;
  q_yaw.setRPY(gimbal.roll, 0.0, gimbal.yaw);
  yaw_t.transform.rotation = tf2::toMsg(q_yaw);

  geometry_msgs::msg::TransformStamped pitch_t;
  pitch_t.header.stamp = stamp;
  pitch_t.header.frame_id = "yaw_link";
  pitch_t.child_frame_id = "pitch_link";
  pitch_t.transform.translation.x = -yaw_to_pitch_distance;
  pitch_t.transform.translation.y = 0.0;
  pitch_t.transform.translation.z = 0.0;
  tf2::Quaternion q_pitch;
  q_pitch.setRPY(0.0, gimbal.pitch, 0.0);
  pitch_t.transform.rotation = tf2::toMsg(q_pitch);

  tf_broadcaster_->sendTransform(yaw_t);
  tf_broadcaster_->sendTransform(pitch_t);
}

void RMSerialDriver::sendArmorData(const rm_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
{
  try {
    SendPacket packet;
    packet.control = msg->control;
    if (!is_open_fire_control)
      packet.fire = 1;
    else
      packet.fire = msg->fire;
    packet.yaw = msg->yaw;
    packet.yaw_vel = msg->yaw_vel;
    packet.yaw_acc = msg->yaw_acc;
    packet.pitch = msg->pitch;
    packet.pitch_vel = msg->pitch_vel;
    packet.pitch_acc = msg->pitch_acc;
    // 20240329 ZY: Eliminate communication latency
    crc16::Append_CRC16_Check_Sum(reinterpret_cast<uint8_t *>(&packet), sizeof(packet));

    std::vector<uint8_t> data = toVector(packet);

    serial_driver_->port()->send(data);
    if (msg->control) {
      std_msgs::msg::Float64 latency;
      latency.data = (this->now() - msg->header.stamp).seconds() * 1000.0;
      RCLCPP_DEBUG_STREAM(get_logger(), "Total latency: " + std::to_string(latency.data) + "ms");
      latency_pub_->publish(latency);
    }
  } catch (const std::exception & ex) {
    RCLCPP_ERROR(get_logger(), "Error while sending data: %s", ex.what());
    reopenPort();
  }
}

void RMSerialDriver::sendBuffData(const rm_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
{
  try {
    SendPacket packet;
    packet.control = msg->control;
    packet.fire = msg->fire;
    packet.yaw = msg->yaw;
    packet.pitch = -msg->pitch;
    packet.yaw_vel = 0.0;
    packet.yaw_acc = 0.0;
    packet.pitch_vel = 0.0;
    packet.pitch_acc = 0.0;
    // 20240329 ZY: Eliminate communication latency
    crc16::Append_CRC16_Check_Sum(reinterpret_cast<uint8_t *>(&packet), sizeof(packet));

    std::vector<uint8_t> data = toVector(packet);

    serial_driver_->port()->send(data);
    if (msg->control) {
      std_msgs::msg::Float64 latency;
      latency.data = (this->now() - msg->header.stamp).seconds() * 1000.0;
      RCLCPP_DEBUG_STREAM(get_logger(), "Total latency: " + std::to_string(latency.data) + "ms");
      latency_pub_->publish(latency);
    }
  } catch (const std::exception & ex) {
    RCLCPP_ERROR(get_logger(), "Error while sending data: %s", ex.what());
    reopenPort();
  }
}

void RMSerialDriver::getParams()
{
  using FlowControl = drivers::serial_driver::FlowControl;
  using Parity = drivers::serial_driver::Parity;
  using StopBits = drivers::serial_driver::StopBits;

  uint32_t baud_rate{};
  auto fc = FlowControl::NONE;
  auto pt = Parity::NONE;
  auto sb = StopBits::ONE;

  try {
    device_name_ = declare_parameter<std::string>("device_name", "");
  } catch (rclcpp::ParameterTypeException & ex) {
    RCLCPP_ERROR(get_logger(), "The device name provided was invalid");
    throw ex;
  }

  try {
    baud_rate = declare_parameter<int>("baud_rate", 0);
  } catch (rclcpp::ParameterTypeException & ex) {
    RCLCPP_ERROR(get_logger(), "The baud_rate provided was invalid");
    throw ex;
  }

  try {
    const auto fc_string = declare_parameter<std::string>("flow_control", "");

    if (fc_string == "none") {
      fc = FlowControl::NONE;
    } else if (fc_string == "hardware") {
      fc = FlowControl::HARDWARE;
    } else if (fc_string == "software") {
      fc = FlowControl::SOFTWARE;
    } else {
      throw std::invalid_argument{
        "The flow_control parameter must be one of: none, software, or "
        "hardware."};
    }
  } catch (rclcpp::ParameterTypeException & ex) {
    RCLCPP_ERROR(get_logger(), "The flow_control provided was invalid");
    throw ex;
  }

  try {
    const auto pt_string = declare_parameter<std::string>("parity", "");

    if (pt_string == "none") {
      pt = Parity::NONE;
    } else if (pt_string == "odd") {
      pt = Parity::ODD;
    } else if (pt_string == "even") {
      pt = Parity::EVEN;
    } else {
      throw std::invalid_argument{"The parity parameter must be one of: none, odd, or even."};
    }
  } catch (rclcpp::ParameterTypeException & ex) {
    RCLCPP_ERROR(get_logger(), "The parity provided was invalid");
    throw ex;
  }

  try {
    const auto sb_string = declare_parameter<std::string>("stop_bits", "");

    if (sb_string == "1" || sb_string == "1.0") {
      sb = StopBits::ONE;
    } else if (sb_string == "1.5") {
      sb = StopBits::ONE_POINT_FIVE;
    } else if (sb_string == "2" || sb_string == "2.0") {
      sb = StopBits::TWO;
    } else {
      throw std::invalid_argument{"The stop_bits parameter must be one of: 1, 1.5, or 2."};
    }
  } catch (rclcpp::ParameterTypeException & ex) {
    RCLCPP_ERROR(get_logger(), "The stop_bits provided was invalid");
    throw ex;
  }

  // 参数表不会存在这个参数。使用calibrate.py启动是为true，其它情况为默认值false
  try {
    this->if_calib = this->declare_parameter<bool>("if_calib", false);
  } catch (rclcpp::ParameterTypeException & ex) {
    RCLCPP_ERROR(this->get_logger(), "The varience \"if_calib\" was invalid");
    throw ex;
  }

  device_config_ =
    std::make_unique<drivers::serial_driver::SerialPortConfig>(baud_rate, fc, pt, sb);
}

void RMSerialDriver::reopenPort()
{
  RCLCPP_WARN(get_logger(), "Attempting to reopen port");
  try {
    if (serial_driver_->port()->is_open()) {
      serial_driver_->port()->close();
    }
    serial_driver_->port()->open();
    RCLCPP_INFO(get_logger(), "Successfully reopened port");
  } catch (const std::exception & ex) {
    RCLCPP_ERROR(get_logger(), "Error while reopening port: %s", ex.what());
    if (rclcpp::ok()) {
      rclcpp::sleep_for(std::chrono::seconds(1));
      reopenPort();
    }
  }
}

void RMSerialDriver::setParam(const rclcpp::Parameter & param)
{
  const auto request_color = static_cast<uint8_t>(param.as_int());
  const auto request_version = detect_color_param_version_.load();

  auto set_detector_param =
    [this, &param, request_color, request_version](
      const char * detector_name, const rclcpp::AsyncParametersClient::SharedPtr & client,
      ResultFuturePtr & future, std::atomic_bool * synced) {
      if (synced->load()) {
        return;
      }

      if (!client->service_is_ready()) {
        if (this->if_calib) return;  // 如果正在标定，跳过报错。不需要改参数，calibrate.py自动处理
        RCLCPP_WARN_THROTTLE(
          get_logger(), *get_clock(), 1000,
          "%s parameter service not ready, skipping detect_color set", detector_name);
        return;
      }

      if (
        future.valid() &&
        future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        return;
      }

      RCLCPP_INFO(
        get_logger(), "Setting %s detect_color to %ld...", detector_name, param.as_int());
      future = client->set_parameters(
      {param}, [this, param, detector_name, request_color, request_version, synced](
                 const ResultFuturePtr & results) {
        for (const auto & result : results.get()) {
          if (!result.successful) {
            RCLCPP_ERROR(
              get_logger(), "Failed to set %s detect_color: %s", detector_name,
              result.reason.c_str());
            return;
          }
        }
        RCLCPP_INFO(
          get_logger(), "Successfully set %s detect_color to %ld!", detector_name,
          param.as_int());
        if (
          request_version == detect_color_param_version_.load() &&
          request_color == desired_detect_color_.load()) {
          synced->store(true);
        }
      });
    };

  set_detector_param(
    "armor_detector", armor_detector_param_client_, armor_set_param_future_,
    &armor_detect_color_synced_);
  set_detector_param(
    "buff_detector", buff_detector_param_client_, buff_set_param_future_,
    &buff_detect_color_synced_);
}

void RMSerialDriver::resetTracker()
{
  if (!reset_tracker_client_->service_is_ready()) {
    RCLCPP_WARN(get_logger(), "Service not ready, skipping tracker reset");
    return;
  }

  auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
  reset_tracker_client_->async_send_request(request);
  RCLCPP_INFO(get_logger(), "Reset tracker!");
}

void RMSerialDriver::changeTarget()
{
  if (!change_target_client_->service_is_ready()) {
    RCLCPP_WARN(get_logger(), "Service not ready, skipping target change");
    return;
  }

  auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
  change_target_client_->async_send_request(request);
  RCLCPP_INFO(get_logger(), "Change target!");
}

}  // namespace rm_serial_driver

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable
// when its library is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rm_serial_driver::RMSerialDriver)
