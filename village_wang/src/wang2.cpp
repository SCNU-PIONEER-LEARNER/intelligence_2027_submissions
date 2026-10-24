#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class SingleDogNode: public rclcpp::Node
{
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;
    /* data */
    void novel_callback(const std_msgs::msg::String::SharedPtr novels)
    {
        RCLCPP_INFO(this->get_logger(),"订阅收到字符串：%s",novels->data.c_str());
    }


public:
    SingleDogNode(std::string name):Node(name)
    {
        RCLCPP_INFO(this->get_logger(),"大家好，我是订阅节点%s",name.c_str());
        sub_novel = this->create_subscription<std_msgs::msg::String>(
    "sexy_girl", 10,
    std::bind(&SingleDogNode::novel_callback, this, _1));


    }

};
int main(int argc,char ** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<SingleDogNode>("wang2");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


