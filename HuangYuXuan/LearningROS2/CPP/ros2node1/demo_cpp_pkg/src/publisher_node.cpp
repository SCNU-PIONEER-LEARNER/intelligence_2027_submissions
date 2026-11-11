#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
using namespace std;
using namespace chrono_literals;

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    // 创建节点和发布者
    auto node = make_shared<rclcpp::Node>("publisher_node");
    auto publisher = node->create_publisher<std_msgs::msg::Float64>("target_x", rclcpp::QoS(10));
    // 定义坐标
    double fake_x = 0.0;
    // 创建定时器，用lambda函数
    auto timer = node->create_wall_timer(1s, [publisher, node, &fake_x]()
                                         {
  // 创建消息包
    auto message = std_msgs::msg::Float64();
 // 假设在移动
 message.data = fake_x;
 fake_x += 5;
//打印日志
                 RCLCPP_INFO_STREAM(node->get_logger(),"目标坐标在:"<<message.data); 
                                                // 发布信息
    publisher->publish(message); });
    RCLCPP_INFO(node->get_logger(), "发布节点启动，正在发布中:");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}