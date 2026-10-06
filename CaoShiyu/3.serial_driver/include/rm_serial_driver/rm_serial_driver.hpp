// Copyright (c) 2022 ChenJun
// Licensed under the Apache-2.0 License.

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

#include "rm_interfaces/msg/gimbal_cmd.hpp"
#include "rm_interfaces/msg/time_info.hpp"
#include "rm_serial_driver/receive_pipeline.hpp"
#include "rm_tools/heartbeat.hpp"

namespace rm_serial_driver
{
class RMSerialDriver : public rclcpp::Node,
                       public IControlActions,
                       public IStatusOutput,
                       public IGimbalOutput
{
public:
  explicit RMSerialDriver(const rclcpp::NodeOptions & options);

  ~RMSerialDriver() override;

private:
  rm_tools::HeartBeatPublisher::SharedPtr heartbeat_;

  void getParams();

  void receiveData();

  void initReceivePipeline();
  void syncDetectColor(uint8_t color) override;
  void publishStatus(const StatusData & status) override;
  void publishGimbal(const GimbalData & gimbal) override;

  std::unique_ptr<ReceivePipeline> receive_pipeline_;

  void sendArmorData(const rm_interfaces::msg::GimbalCmd::ConstSharedPtr msg);
  void sendBuffData(const rm_interfaces::msg::GimbalCmd::ConstSharedPtr msg);

  // void sendArmorData(
  //   const rm_interfaces::msg::GimbalCmd::ConstSharedPtr msg,
  //   const rm_interfaces::msg::TimeInfo::ConstSharedPtr time_info);

  void reopenPort();

  void setParam(const rclcpp::Parameter & param);

  void resetTracker() override;

  void changeTarget() override;

  bool is_open_fire_control;
  // Serial port
  std::unique_ptr<IoContext> owned_ctx_;
  std::string device_name_;
  std::unique_ptr<drivers::serial_driver::SerialPortConfig> device_config_;
  std::unique_ptr<drivers::serial_driver::SerialDriver> serial_driver_;

  // Param clients to set detect_color
  using ResultFuturePtr = std::shared_future<std::vector<rcl_interfaces::msg::SetParametersResult>>;
  std::atomic_bool has_desired_detect_color_{false};
  std::atomic<uint8_t> desired_detect_color_{0};
  std::atomic_size_t detect_color_param_version_{0};
  std::atomic_bool armor_detect_color_synced_{false};
  std::atomic_bool buff_detect_color_synced_{false};
  rclcpp::AsyncParametersClient::SharedPtr armor_detector_param_client_;
  rclcpp::AsyncParametersClient::SharedPtr buff_detector_param_client_;
  ResultFuturePtr armor_set_param_future_;
  ResultFuturePtr buff_set_param_future_;

  // Service client to reset tracker
  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr reset_tracker_client_;

  // Service client to change target
  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr change_target_client_;

  // Aimimg point receiving from serial port for visualization
  visualization_msgs::msg::Marker aiming_point_;

  // Broadcast tf from odom to gimbal_link
  double timestamp_offset_ = 0;
  double yaw_to_pitch_distance_ = 0.0;
  double pitch_to_gun_x_ = 0.0;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  rclcpp::Subscription<rm_interfaces::msg::GimbalCmd>::SharedPtr aim_sub_;
  rclcpp::Subscription<rm_interfaces::msg::GimbalCmd>::SharedPtr buff_sub_;


  // message_filters::Subscriber<rm_interfaces::msg::GimbalCmd> aim_sub_;
  // message_filters::Subscriber<rm_interfaces::msg::TimeInfo> aim_time_info_sub_;

  // typedef message_filters::sync_policies::ApproximateTime<
  //   rm_interfaces::msg::GimbalCmd, rm_interfaces::msg::TimeInfo>
  //   aim_syncpolicy;
  // typedef message_filters::Synchronizer<aim_syncpolicy> AimSync;
  // std::shared_ptr<AimSync> aim_sync_;

  // For debug usage
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr latency_pub_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;

  std::thread receive_thread_;

  // Task message
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr task_pub_;

  // bullet speed message
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr bullet_speed_pub_;

  // Time message
  rclcpp::Publisher<rm_interfaces::msg::TimeInfo>::SharedPtr aim_time_info_pub_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record_controller_pub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr is_play_pub_;

  bool if_calib = false;  // 给标定节点用的，不用管
};
}  // namespace rm_serial_driver

#endif  // RM_SERIAL_DRIVER__RM_SERIAL_DRIVER_HPP_
