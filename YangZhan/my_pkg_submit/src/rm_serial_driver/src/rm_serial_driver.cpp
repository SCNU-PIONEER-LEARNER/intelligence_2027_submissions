// RMSerialDriver 实现：参数读取、串口收发、消费者注册。

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
  RCLCPP_INFO(this->get_logger(), "Start RMSerialDriver!");

  getParams();

  is_open_fire_control = this->declare_parameter("is_open_fire_control", true);

  timestamp_offset_ = this->declare_parameter("timestamp_offset", 0.0);

  yaw_to_pitch_distance_ = this->declare_parameter("yaw_to_pitch_distance", 0.0);

  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

  task_pub_ = this->create_publisher<std_msgs::msg::String>("task_mode", 10);
  latency_pub_ = this->create_publisher<std_msgs::msg::Float64>("latency", 10);
  bullet_speed_pub_ = this->create_publisher<std_msgs::msg::Float32>("bullet_speed", 10);
  record_controller_pub_ = this->create_publisher<std_msgs::msg::String>("record_controller", 10);
  is_play_pub_ = this->create_publisher<std_msgs::msg::Bool>("is_play", 10);

  aim_time_info_pub_ =
    this->create_publisher<rm_interfaces::msg::TimeInfo>("time_info/aim", 10);

  reset_tracker_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/reset");

  change_target_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/change");

  registerConsumers();

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
  data.reserve(sizeof(ReceivePacket));

  while (rclcpp::ok()) {
    try {

      serial_driver_->port()->receive(header);

      if (header[0] == 0x5A) {

        data.resize(sizeof(ReceivePacket) - 1);
        serial_driver_->port()->receive(data);

        data.insert(data.begin(), header[0]);

        ReceivePacket packet = fromVector(data);

        bool crc_ok =
          crc16::Verify_CRC16_Check_Sum(reinterpret_cast<const uint8_t *>(&packet), sizeof(packet));
        if (crc_ok) {

          for (auto & consumer : consumers_){
            consumer->consume(packet);
          }
        }
          } else {

        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 20, "Invalid header: %02X", header[0]);
      }
      } catch (const std::exception & ex) {

      RCLCPP_ERROR_THROTTLE(
        get_logger(), *get_clock(), 20, "Error while receiving data: %s", ex.what());
      reopenPort();
    }
  }
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


}

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(rm_serial_driver::RMSerialDriver)
