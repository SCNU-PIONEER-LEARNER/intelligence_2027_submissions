#ifndef RM_SERIAL_DRIVER__INFANTRY_PROTOCOL_HPP_
#define RM_SERIAL_DRIVER__INFANTRY_PROTOCOL_HPP_
#include "auto_aim_interfaces/msg/gimbal_cmd.hpp"
#include "rm_serial_driver/packet_codec.hpp"
#include <memory> 
#include <atomic> 
#include <future>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/string.hpp>
#include "std_srvs/srv/trigger.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "rm_serial_driver/protocol.hpp"
#include "auto_aim_interfaces/msg/time_info.hpp"
#include <vector>
#include <cstdint>
namespace rm_serial_driver
{
    class InfantryProtocol:public Protocol
    { 
        public:
        InfantryProtocol(rclcpp::Node * node);
        ~InfantryProtocol() {};
        //全是复制粘贴，纯体力活,把步兵协议相关的全抄进来
        void receiveTopic(const uint8_t * data, size_t len) override;
        std::vector<uint8_t> sendArmorData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)override;
        std::vector<uint8_t> sendBuffData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)override;
        void onReceivePacket(const ControllerCommand & packet);
        void handleControllerTaskCmd(const ControllerCommand & packet);
        void publishControllerAuxData(const ControllerCommand & packet);
        void publishGimbalTf(const ControllerCommand & packet);
        void syncDetectColorRequest(uint8_t color);
        void setParam(const rclcpp::Parameter & param);
        void resetTracker();
        void changeTarget();
        PacketCodec codec_;
        bool is_open_fire_control=true;
        bool if_calib = false;
        double timestamp_offset_ = 0.0;
        double yaw_to_pitch_distance_ = 0.0;
        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr task_pub_;
        rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr bullet_speed_pub_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record_controller_pub_;
        rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr is_play_pub_;
        rclcpp::Publisher<auto_aim_interfaces::msg::TimeInfo>::SharedPtr aim_time_info_pub_;
        using ResultFuturePtr = std::shared_future<std::vector<rcl_interfaces::msg::SetParametersResult>>;
        rclcpp::AsyncParametersClient::SharedPtr armor_detector_param_client_;
        rclcpp::AsyncParametersClient::SharedPtr buff_detector_param_client_;
        std::atomic_bool has_desired_detect_color_=false;
        std::atomic<uint8_t> desired_detect_color_=0;
        std::atomic_size_t detect_color_param_version_=0;
        std::atomic_bool armor_detect_color_synced_=false;
        std::atomic_bool buff_detect_color_synced_=false;
        ResultFuturePtr armor_set_param_future_;
        ResultFuturePtr buff_set_param_future_;
        rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr reset_tracker_client_;
        rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr change_target_client_;
    };    
}
#endif 