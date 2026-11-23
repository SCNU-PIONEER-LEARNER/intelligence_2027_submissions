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
#include <std_msgs/msg/u_int8_multi_array.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <visualization_msgs/msg/marker.hpp>

#include "auto_aim_interfaces/msg/gimbal_cmd.hpp"
#include "auto_aim_interfaces/msg/time_info.hpp"
#include "rm_serial_driver/packet.hpp"
#include "rm_tools/heartbeat.hpp"

namespace rm_serial_driver
{
// 接收包的字段视图：每个结构体只包含一个职责所需的字段，
// 处理函数只依赖这些视图，不依赖 ReceivePacket 的整体布局
struct DetectColorField
{
  uint8_t color;
};

struct TrackerCommandField
{
  bool reset;
  bool change;
};

struct TaskModeField
{
  uint8_t mode;
};

struct BulletSpeedField
{
  float speed;
};

struct PlayStateField
{
  bool is_play;
};

struct GimbalStateField
{
  float roll;
  float pitch;
  float yaw;
};

class RMSerialDriver : public rclcpp::Node
{
public:
  explicit RMSerialDriver(const rclcpp::NodeOptions & options);

  ~RMSerialDriver() override;

private:
  rm_tools::HeartBeatPublisher::SharedPtr heartbeat_;

  void getParams();

  void receiveData();

  // 收包与校验：读一帧并做 CRC 校验，失败时已打印日志
  bool receivePacket(ReceivePacket & packet);

  // 虚拟下位机模式：订阅 /receive_packet，校验后走同一条分发链路
  void receiveVirtualFrame(const std_msgs::msg::UInt8MultiArray::ConstSharedPtr msg);

  // 全工程唯一读取 ReceivePacket 字段的地方
  void dispatchReceivePacket(const ReceivePacket & packet);

  // 各职责处理函数：只接收自己需要的字段视图
  void onDetectColor(const DetectColorField & field);
  void onTrackerCommand(const TrackerCommandField & field);
  void onTaskMode(const TaskModeField & field);
  void onBulletSpeed(const BulletSpeedField & field);
  void onPlayState(const PlayStateField & field);
  void onGimbalState(const GimbalStateField & field);

  void sendArmorData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg);
  void sendBuffData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg);

  // 下行帧出口：真机写串口，虚拟模式发布到 /send_packet
  void sendSerialPacket(const std::vector<uint8_t> & data);

  // void sendArmorData(
  //   const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg,
  //   const auto_aim_interfaces::msg::TimeInfo::ConstSharedPtr time_info);

  void reopenPort();

  void setParam(const rclcpp::Parameter & param);

  void resetTracker();

  void changeTarget();

  bool is_open_fire_control;
  bool use_virtual_low_board_ = false;
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

  rclcpp::Subscription<auto_aim_interfaces::msg::GimbalCmd>::SharedPtr aim_sub_;
  rclcpp::Subscription<auto_aim_interfaces::msg::GimbalCmd>::SharedPtr buff_sub_;

  // 虚拟下位机：收 /receive_packet，发 /send_packet
  rclcpp::Subscription<std_msgs::msg::UInt8MultiArray>::SharedPtr virtual_packet_sub_;
  rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr send_packet_pub_;


  // message_filters::Subscriber<auto_aim_interfaces::msg::GimbalCmd> aim_sub_;
  // message_filters::Subscriber<auto_aim_interfaces::msg::TimeInfo> aim_time_info_sub_;

  // typedef message_filters::sync_policies::ApproximateTime<
  //   auto_aim_interfaces::msg::GimbalCmd, auto_aim_interfaces::msg::TimeInfo>
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
  rclcpp::Publisher<auto_aim_interfaces::msg::TimeInfo>::SharedPtr aim_time_info_pub_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record_controller_pub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr is_play_pub_;

  bool if_calib = false;  // 给标定节点用的，不用管
};
}  // namespace rm_serial_driver

#endif  // RM_SERIAL_DRIVER__RM_SERIAL_DRIVER_HPP_
