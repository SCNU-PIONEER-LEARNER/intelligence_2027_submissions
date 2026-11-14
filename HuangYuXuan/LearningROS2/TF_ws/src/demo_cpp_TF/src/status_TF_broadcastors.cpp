#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"//消息接口
#include "tf2/LinearMath/Quaternion.hpp"//四元数数学库
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"//消息类型转换
#include "tf2_ros/static_transform_broadcaster.hpp"//静态坐标广播器
using namespace std;
class StatusTFbroadcastors:public rclcpp::Node{

public:

    StatusTFbroadcastors():Node("status_TF_broadcastors"){
    //实例化
    broadcastor =std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);//类似于实例publisher
    this->tf_cast();
    }
    void tf_cast(){
    auto launch =geometry_msgs::msg::TransformStamped() ;
    launch.header.stamp =this->get_clock()->now();
    launch.header.frame_id="map";
    launch.child_frame_id="target_point";
    launch.transform.translation.x =5.0;
    launch.transform.translation.y =3.0;
    launch.transform.translation.z =0.0;
    //定义一个四元数类的变量
    tf2::Quaternion q;
    q.setRPY(0.0,0.0,60*M_PI/180);
    launch.transform.rotation = tf2::toMsg(q);
    this->broadcastor->sendTransform(launch);
}

private:
    //初始化
    shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcastor;
};

int main(int argc,char**argv){
    rclcpp::init(argc,argv);
    auto node =std::make_shared<StatusTFbroadcastors>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}