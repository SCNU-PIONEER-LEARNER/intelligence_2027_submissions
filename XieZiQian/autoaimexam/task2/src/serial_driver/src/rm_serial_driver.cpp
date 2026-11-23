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

  use_virtual_low_board_ = this->declare_parameter("use_virtual_low_board", false);

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
    this->create_publisher<auto_aim_interfaces::msg::TimeInfo>("time_info/aim", 10);

  // Detect parameter clients
  armor_detector_param_client_ =
    std::make_shared<rclcpp::AsyncParametersClient>(this, "armor_detector");
  buff_detector_param_client_ =
    std::make_shared<rclcpp::AsyncParametersClient>(this, "buff_detector");

  // Tracker reset service client
  reset_tracker_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/reset");

  // Target change service cilent
  change_target_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/change");

  if (use_virtual_low_board_) {
    // 数据源来自虚拟下位机话题，不打开串口，也不起收包线程
    virtual_packet_sub_ = this->create_subscription<std_msgs::msg::UInt8MultiArray>(
      "/receive_packet", rclcpp::QoS(10).reliable(),
      std::bind(&RMSerialDriver::receiveVirtualFrame, this, std::placeholders::_1));
    send_packet_pub_ = this->create_publisher<std_msgs::msg::UInt8MultiArray>("/send_packet", 10);
    RCLCPP_INFO(
      get_logger(),
      "Virtual low board mode: receiving '/receive_packet', publishing '/send_packet', "
      "serial port skipped");
  } else {
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
  }

  // Create Subscription
  aim_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalCmd>(
    "armor_planner/cmd_gimbal", rclcpp::SensorDataQoS(),
    std::bind(&RMSerialDriver::sendArmorData, this, std::placeholders::_1));
  buff_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalCmd>(
    "buff_tracker/cmd_gimbal", rclcpp::SensorDataQoS(),
    std::bind(&RMSerialDriver::sendBuffData, this, std::placeholders::_1));
  heartbeat_ = rm_tools::HeartBeatPublisher::create(this);
}
RMSerialDriver::~RMSerialDriver()
{
  if (receive_thread_.joinable()) {
    receive_thread_.join();
  }

  if (serial_driver_->port() != nullptr && serial_driver_->port()->is_open()) {
    serial_driver_->port()->close();
  }

  if (owned_ctx_) {
    owned_ctx_->waitForExit();
  }
}

void RMSerialDriver::receiveData()
{
  ReceivePacket packet;

  while (rclcpp::ok()) {
    try {
      if (!receivePacket(packet)) {
        continue;
      }
      dispatchReceivePacket(packet);
    } catch (const std::exception & ex) {
      RCLCPP_ERROR_THROTTLE(
        get_logger(), *get_clock(), 20, "Error while receiving data: %s", ex.what());
      reopenPort();
    }
  }
}

bool RMSerialDriver::receivePacket(ReceivePacket & packet)
{
  std::vector<uint8_t> header(1);
  serial_driver_->port()->receive(header);
  if (header[0] != 0x5A) {
    RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 20, "Invalid header: %02X", header[0]);
    return false;
  }

  std::vector<uint8_t> payload(sizeof(ReceivePacket) - 1);
  serial_driver_->port()->receive(payload);

  std::vector<uint8_t> data;
  data.reserve(sizeof(ReceivePacket));
  data.push_back(header[0]);
  data.insert(data.end(), payload.begin(), payload.end());

  packet = fromVector(data);

  if (!crc16::Verify_CRC16_Check_Sum(
        reinterpret_cast<const uint8_t *>(&packet), sizeof(packet))) {
    RCLCPP_ERROR(get_logger(), "CRC error!");
    return false;
  }
  return true;
}

//接收虚拟下位机
void RMSerialDriver::receiveVirtualFrame(
  const std_msgs::msg::UInt8MultiArray::ConstSharedPtr msg)
{
  const auto & data = msg->data;
  if (data.size() != sizeof(ReceivePacket)) {
    RCLCPP_WARN_THROTTLE(
      get_logger(), *get_clock(), 1000, "Virtual packet rejected: size=%zu, expected %zu",
      data.size(), sizeof(ReceivePacket));
    return;
  }

  if (!crc16::Verify_CRC16_Check_Sum(data.data(), static_cast<uint32_t>(data.size()))) {
    RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 1000, "Virtual packet CRC error!");
    return;
  }

  dispatchReceivePacket(fromVector(data));
}

//ReceivePacket字段增删只需改这里
void RMSerialDriver::dispatchReceivePacket(const ReceivePacket & packet)
{
  onDetectColor(DetectColorField{static_cast<uint8_t>(packet.detect_color)});
  onTrackerCommand(TrackerCommandField{packet.reset_tracker, packet.change_target});
  onTaskMode(TaskModeField{packet.task_mode});
  onBulletSpeed(BulletSpeedField{packet.bullet_speed});
  onPlayState(PlayStateField{packet.is_play != 0});
  onGimbalState(GimbalStateField{packet.roll, packet.pitch, packet.yaw});
}

void RMSerialDriver::onDetectColor(const DetectColorField & field)
{
  if (
    !has_desired_detect_color_.load() ||
    field.color != desired_detect_color_.load()) {
    desired_detect_color_.store(field.color);
    has_desired_detect_color_.store(true);
    detect_color_param_version_.fetch_add(1);
    armor_detect_color_synced_.store(false);
    buff_detect_color_synced_.store(false);
  }

  if (
    !armor_detect_color_synced_.load() ||
    !buff_detect_color_synced_.load()) {
    setParam(rclcpp::Parameter("detect_color", static_cast<int>(field.color)));
  }
}

void RMSerialDriver::onTrackerCommand(const TrackerCommandField & field)
{
  if (field.reset) {
    resetTracker();
  }

  if (field.change) {
    changeTarget();
  }
}

void RMSerialDriver::onTaskMode(const TaskModeField & field)
{
  std_msgs::msg::String task;
  if (field.mode == 0)
    task.data = "aim";
  else if (field.mode == 1)
    task.data = "small_buff";
  else if (field.mode == 2)
    task.data = "large_buff";
  // 保底是自瞄
  else
    task.data = "aim";
  task_pub_->publish(task);
}

void RMSerialDriver::onBulletSpeed(const BulletSpeedField & field)
{
  std_msgs::msg::Float32 bullet_speed;
  bullet_speed.data = field.speed;
  bullet_speed_pub_->publish(bullet_speed);
}

void RMSerialDriver::onPlayState(const PlayStateField & field)
{
  std_msgs::msg::String record_controller;
  record_controller.data = field.is_play ? "start" : "stop";
  record_controller_pub_->publish(record_controller);

  std_msgs::msg::Bool is_play;
  is_play.data = field.is_play;
  is_play_pub_->publish(is_play);
}

void RMSerialDriver::onGimbalState(const GimbalStateField & field)
{
  geometry_msgs::msg::TransformStamped yaw_t;
  timestamp_offset_ = this->get_parameter("timestamp_offset").as_double();
  auto stamp = this->now() - rclcpp::Duration::from_seconds(timestamp_offset_);
  const double yaw_to_pitch_distance =
    this->get_parameter("yaw_to_pitch_distance").as_double();

  // odom -> yaw_link: yaw axis rotation
  yaw_t.header.stamp = stamp;
  yaw_t.header.frame_id = "odom";
  yaw_t.child_frame_id = "yaw_link";
  yaw_t.transform.translation.x = 0.0;
  yaw_t.transform.translation.y = 0.0;
  yaw_t.transform.translation.z = 0.0;
  tf2::Quaternion q_yaw;
  q_yaw.setRPY(field.roll, 0.0, field.yaw);
  yaw_t.transform.rotation = tf2::toMsg(q_yaw);

  // yaw_link -> pitch_link: fixed axis offset + pitch axis rotation
  geometry_msgs::msg::TransformStamped pitch_t;
  pitch_t.header.stamp = stamp;
  pitch_t.header.frame_id = "yaw_link";
  pitch_t.child_frame_id = "pitch_link";
  pitch_t.transform.translation.x = -yaw_to_pitch_distance;
  pitch_t.transform.translation.y = 0.0;
  pitch_t.transform.translation.z = 0.0;
  tf2::Quaternion q_pitch;
  q_pitch.setRPY(0.0, field.pitch, 0.0);
  pitch_t.transform.rotation = tf2::toMsg(q_pitch);

  tf_broadcaster_->sendTransform(yaw_t);
  tf_broadcaster_->sendTransform(pitch_t);
}

void RMSerialDriver::sendSerialPacket(const std::vector<uint8_t> & data)
{
  if (use_virtual_low_board_) {
    std_msgs::msg::UInt8MultiArray message;
    message.data = data;
    send_packet_pub_->publish(message);
    return;
  }

  serial_driver_->port()->send(data);
}

void RMSerialDriver::sendArmorData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
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

    sendSerialPacket(data);
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

void RMSerialDriver::sendBuffData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
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

    sendSerialPacket(data);
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
  if (use_virtual_low_board_) {
    return;
  }

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
