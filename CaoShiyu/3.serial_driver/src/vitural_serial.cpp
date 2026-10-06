// Created by Chengfu Zou
// Copyright (C) FYT Vision Group. All rights reserved.

#include <tf2_ros/transform_broadcaster.h>

#include <chrono>
#include <condition_variable>
#include <future>
#include <memory>
#include <thread>
#include <vector>

#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rclcpp/executors.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/node_options.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/string.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

namespace rm_serial_driver
{
class VirtualSerialNode : public rclcpp::Node
{
public:
  explicit VirtualSerialNode(const rclcpp::NodeOptions & options) : Node("serial_driver", options)
  {
    RCLCPP_INFO(this->get_logger(), "Starting VirtualSerialNode!");

    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    task_pub_ = this->create_publisher<std_msgs::msg::String>("task_mode", 10);
    record_controller_pub_ =
      this->create_publisher<std_msgs::msg::String>("record_controller", 10);
    is_play_pub_ = this->create_publisher<std_msgs::msg::Bool>("is_play", 10);
    task = this->declare_parameter("task_mode", "aim");
    timestamp_offset_ = this->declare_parameter("timestamp_offset", 0.0);
    yaw_to_pitch_distance_ = this->declare_parameter("yaw_to_pitch_distance", 0.0);

    yaw_ = this->declare_parameter("virtual_yaw", 0.0);
    pitch_ = this->declare_parameter("virtual_pitch", 0.0);
    virtual_is_play_ = this->declare_parameter("virtual_is_play", false);

    timer_ = this->create_wall_timer(std::chrono::milliseconds(1), [this]() {
      task = this->get_parameter("task_mode").as_string();
      std_msgs::msg::String task_msg;
      task_msg.data = task;
      task_pub_->publish(task_msg);

      virtual_is_play_ = this->get_parameter("virtual_is_play").as_bool();
      std_msgs::msg::String record_controller;
      record_controller.data = virtual_is_play_ ? "start" : "stop";
      record_controller_pub_->publish(record_controller);
      std_msgs::msg::Bool is_play;
      is_play.data = virtual_is_play_;
      is_play_pub_->publish(is_play);

      timestamp_offset_ = this->get_parameter("timestamp_offset").as_double();
      auto stamp = this->now() - rclcpp::Duration::from_seconds(timestamp_offset_);

      const double yaw_to_pitch_distance =
        this->get_parameter("yaw_to_pitch_distance").as_double();

      yaw_ = this->get_parameter("virtual_yaw").as_double();
      pitch_ = this->get_parameter("virtual_pitch").as_double();

      geometry_msgs::msg::TransformStamped yaw_t;
      yaw_t.header.stamp = stamp;
      yaw_t.header.frame_id = "odom";
      yaw_t.child_frame_id = "yaw_link";
      yaw_t.transform.translation.x = 0.0;
      yaw_t.transform.translation.y = 0.0;
      yaw_t.transform.translation.z = 0.0;
      tf2::Quaternion q_yaw;
      q_yaw.setRPY(0.0, 0.0, yaw_);
      yaw_t.transform.rotation = tf2::toMsg(q_yaw);

      geometry_msgs::msg::TransformStamped pitch_t;
      pitch_t.header.stamp = stamp;
      pitch_t.header.frame_id = "yaw_link";
      pitch_t.child_frame_id = "pitch_link";
      pitch_t.transform.translation.x = -yaw_to_pitch_distance;
      pitch_t.transform.translation.y = 0.0;
      pitch_t.transform.translation.z = 0.0;
      tf2::Quaternion q_pitch;
      q_pitch.setRPY(0.0, pitch_, 0.0);
      pitch_t.transform.rotation = tf2::toMsg(q_pitch);

      tf_broadcaster_->sendTransform(yaw_t);
      tf_broadcaster_->sendTransform(pitch_t);
    });
  }

private:
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr task_pub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record_controller_pub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr is_play_pub_;
  std::string task;
  double yaw_to_pitch_distance_ = 0.0;
  double timestamp_offset_ = 0.0;
  double yaw_ = 0.0;
  double pitch_ = 0.0;
  bool virtual_is_play_ = false;
};
}  // namespace rm_serial_driver

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rm_serial_driver::VirtualSerialNode)
