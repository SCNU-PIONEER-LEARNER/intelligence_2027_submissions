// Copyright (c) 2022 ChenJun
// Licensed under the Apache-2.0 License.
// RMSerialDriver 节点声明。
// 负责串口收发：接收下位机报文并发布话题/服务/TF，同时把云台指令写回串口。

#ifndef RM_SERIAL_DRIVER__RM_SERIAL_DRIVER_HPP_
#define RM_SERIAL_DRIVER__RM_SERIAL_DRIVER_HPP_

#include <message_filters/subscriber.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <tf2_ros/transform_broadcaster.h>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>

#include <serial_driver/serial_driver.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <visualization_msgs/msg/marker.hpp>

#include "packet.hpp"
#include "rm_interfaces/msg/gimbal_cmd.hpp"
#include "rm_interfaces/msg/time_info.hpp"
#include "rm_tools/heartbeat.hpp"

#include "rm_serial_driver/receive_consumer.hpp"
#include "rm_serial_driver/packet.hpp"

namespace rm_serial_driver
{

class RMSerialDriver : public rclcpp::Node
{
public:

  explicit RMSerialDriver(const rclcpp::NodeOptions & options);

  ~RMSerialDriver() override;

private:

  void registerConsumers();
  std::vector<std::unique_ptr<ReceiveConsumer>> consumers_;

  void getParams();

  void receiveData();

  void sendArmorData(const rm_interfaces::msg::GimbalCmd::ConstSharedPtr msg);

  void sendBuffData(const rm_interfaces::msg::GimbalCmd::ConstSharedPtr msg);

  void reopenPort();

  void setParam(const rclcpp::Parameter & param);

  void resetTracker();

  void changeTarget();

  rm_tools::HeartBeatPublisher::SharedPtr heartbeat_;

  std::unique_ptr<IoContext> owned_ctx_;
  std::string device_name_;

  std::unique_ptr<drivers::serial_driver::SerialPortConfig> device_config_;

  std::unique_ptr<drivers::serial_driver::SerialDriver> serial_driver_;

  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr reset_tracker_client_;
  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr change_target_client_;

  visualization_msgs::msg::Marker aiming_point_;

  double pitch_to_gun_x_ = 0.0;
  double timestamp_offset_ = 0.0;
  double yaw_to_pitch_distance_ = 0.0;
  bool is_open_fire_control = true;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  rclcpp::Subscription<rm_interfaces::msg::GimbalCmd>::SharedPtr aim_sub_;
  rclcpp::Subscription<rm_interfaces::msg::GimbalCmd>::SharedPtr buff_sub_;

  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr latency_pub_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;

  std::thread receive_thread_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr task_pub_;

  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr bullet_speed_pub_;

  rclcpp::Publisher<rm_interfaces::msg::TimeInfo>::SharedPtr aim_time_info_pub_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record_controller_pub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr is_play_pub_;

  bool if_calib = false;
};

}

#endif
