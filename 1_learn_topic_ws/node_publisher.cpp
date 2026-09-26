#include<rclcpp/rclcpp.hpp>
#include<std_msgs/msg/string.hpp>
#include<chrono>
class publisher1 :public rclcpp::Node
{
    public:
    publisher1():Node("publisher1")
    {
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic1",10);
        timer_ = this->create_wall_timer(std::chrono::seconds(1),std::bind(&publisher1::timer_callback,this));
    }
    private:
    void timer_callback()
    {
        auto message = std_msgs::msg::String();
        message.data = "can you hear me?";
        RCLCPP_INFO(this->get_logger(),"Publishing: '%s'",message.data.c_str());
        publisher_->publish(message);
    }
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<publisher1>());
    rclcpp::shutdown();
    return 0;
};