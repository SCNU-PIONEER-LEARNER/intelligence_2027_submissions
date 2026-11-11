#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "status_interface/msg/system_status.hpp" 

using namespace std::chrono_literals;
using SystemStatus = status_interface::msg::SystemStatus;

class SystemStatusPublisher : public rclcpp::Node
{
public:
    SystemStatusPublisher() : Node("SystemStatusPublisher")
    {
        // 声明并初始化
        publisher_ = this->create_publisher<SystemStatus>("sys_status", rclcpp::QoS(10));
        // 注意这里的 std::bind 写法，没有多余的逗号
        timer_ = this->create_wall_timer(1s, std::bind(&SystemStatusPublisher::timer_callback, this));
    }

private:
    void timer_callback()
    {
        auto msg = SystemStatus();
        msg.stamp = this->get_clock()->now();
        msg.host_name = "Ubuntu-PC";
        msg.cpu_percent = 35.5;
        msg.memory_percent = 45.0;
        RCLCPP_INFO(this->get_logger(), "发送数据: CPU=%.1f, 内存=%.1f", msg.cpu_percent, msg.memory_percent);
        
        // 发布
        publisher_->publish(msg);
    }

    // 成员变量声明（必须放在类里面）
    rclcpp::Publisher<SystemStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SystemStatusPublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}