#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"//消息接口
#include "tf2/LinearMath/Quaternion.hpp"//四元数库
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"//消息类型转换
#include "tf2_ros/transform_broadcaster.hpp"//坐标广播器
#include "chrono"
using namespace std::chrono_literals;//可以使用s,ms等
using namespace std;

class DynamicTf2Broadcastors: public rclcpp::Node{
private:
    //初始化
    shared_ptr<tf2_ros::TransformBroadcaster> broadcastor_;
    rclcpp::TimerBase::SharedPtr timer_;
public:
    DynamicTf2Broadcastors():Node ("dynamic_tf2_broadcastors"){
        //实例化
        this ->broadcastor_ = std::make_shared<tf2_ros::TransformBroadcaster> (this);
        // this->tf2_transform();
        this->timer_ =create_wall_timer(10ms,bind(&DynamicTf2Broadcastors::tf2_transform,this));
    }

    void tf2_transform(){
        auto msg =geometry_msgs::msg::TransformStamped();
        msg.header.stamp=this->get_clock()->now();
        msg.header.frame_id ="map";
        msg.child_frame_id ="base_link";
        msg.transform.translation.x=5.0;
        msg.transform.translation.y=3.0;
        msg.transform.translation.z=1.0;
        tf2::Quaternion q;
        q.setRPY(0.0,0.0,30*M_PI/180);//欧拉角转四元数
        msg.transform.rotation = tf2::toMsg(q);//转为消息接口类型
        this->broadcastor_->sendTransform(msg);
        
    }
};

int main(int argc,char** argv){
    rclcpp::init(argc,argv);
    auto node = make_shared<DynamicTf2Broadcastors>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

