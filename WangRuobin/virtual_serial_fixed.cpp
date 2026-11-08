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

namespace rm_serial_driver;


class TaskStatusPublisher 
{
public:
TaskStatusPublisher(rclcpp::Node* node){
    task_pub_ = node->create_publisher<std_msgs::msg::String>("task_mode", 10);
    record_controller_pub_ =node->create_publisher<std_msgs::msg::String>("record_controller", 10);
    is_play_pub_ = node->create_publisher<std_msgs::msg::Bool>("is_play", 10);4

}
    void TSPupdate(rclcpp::Node* node){
        
        task = node->get_parameter("task_mode").as_string();
        std_msgs::msg::String task_msg;
        task_msg.data = task;
        task_pub_->publish(task_msg);

        virtual_is_play_ = node->get_parameter("virtual_is_play").as_bool();
        std_msgs::msg::String record_controller;
        record_controller.data = virtual_is_play_ ? "start" : "stop";
        record_controller_pub_->publish(record_controller);
        std_msgs::msg::Bool is_play;
        is_play.data = virtual_is_play_;
        is_play_pub_->publish(is_play);
}
private:
    std::string task;
    bool virtual_is_play_ = false;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr task_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record_controller_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr is_play_pub_;
};
//负责发送任务和状态的话题

class TFBroadcaster 
{
public:
TFBroadcaster(rclcpp::Node* node){
    tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*node);
    
}
    void TFupdate(rclcpp::Node* node){
        timestamp_offset_ = node->get_parameter("timestamp_offset").as_double();
    auto stamp = node->now() - rclcpp::Duration::from_seconds(timestamp_offset_);

    const double yaw_to_pitch_distance =
        node->get_parameter("yaw_to_pitch_distance").as_double();

    yaw_ = node->get_parameter("virtual_yaw").as_double();
    pitch_ = node->get_parameter("virtual_pitch").as_double();

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
    }
private:
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};
//负责坐标系计算和发送
class VirtualSerialNode :public rclcpp::Node
{
public:
    explicit VirtualSerialNode(const rclcpp::NodeOptions & options) : Node("serial_driver", options){
    RCLCPP_INFO(this->get_logger(), "Starting VirtualSerialNode!");
    
    this->declare_parameter("task_mode", "aim");
    this->declare_parameter("timestamp_offset", 0.0);
    this->declare_parameter("yaw_to_pitch_distance", 0.0);

    this->declare_parameter("virtual_yaw", 0.0);
    this->declare_parameter("virtual_pitch", 0.0);
    this->declare_parameter("virtual_is_play", false);

    task_publisher_ =std::make_unique<TaskStatusPublisher>(this);
    tf_calculator_ =std::make_unique<TFBroadcaster>(this);


    timer_=this->create_wall_timer(std::chrono::milliseconds(10),[this](){
        task_publisher_->TSPupdate(this);
        tf_calculator_->TFupdate(this);
    })
    }
private:
    std::unique_ptr<TaskStatusPublisher> task_publisher_;
    std::unique_ptr<TFBroadcaster> tf_calculator_;
    rclcpp::TimerBase::SharedPtr timer_;
};
//负责统筹调用以下两个类

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(rm_serial_driver::VirtualSerialNode)

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rm_serial_driver::VirtualSerialNode>(rclcpp::NodeOptions());
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}