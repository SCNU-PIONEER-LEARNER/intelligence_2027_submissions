#include<rclcpp/rclcpp.hpp>
#include<std_msgs/msg/string.hpp>
class subscriber1:public rclcpp::Node
{
    public:
    subscriber1():Node("subscriber1")
    {
        subscription_ = this->create_subscription<std_msgs::msg::String>("topic1",10,std::bind(&subscriber1::topic_callback,this,std::placeholders::_1));
    }
    private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        RCLCPP_INFO(this->get_logger(),"I heard: '%s'",msg->data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};
int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<subscriber1>());
    rclcpp::shutdown();
    return 0;
};