#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
using namespace std;

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = make_shared<rclcpp::Node>("subscriber_node");
    auto subscriber = node->create_subscription<std_msgs::msg::Float64>("target_x", rclcpp::QoS(10), [node](const std_msgs::msg::Float64::SharedPtr msg)
                                                                        { RCLCPP_INFO_STREAM(node->get_logger(), "收到目标坐标:" << msg->data); });
    RCLCPP_INFO_STREAM(node->get_logger(), "订阅节点启动,正在订阅中");
    rclcpp::spin(node);
    rclcpp::shutdown();
}