#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"//消息接口
#include "tf2/LinearMath/Quaternion.hpp"//四元数库,将欧拉角转为四元数
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"//消息类型转换
#include "tf2_ros/transform_listener.hpp"//接收坐标信息
#include "chrono"
#include "tf2/utils.hpp"//将四元数转为欧拉角
#include "tf2_ros/buffer.hpp"//提供Buffer
using namespace std::chrono_literals;//可以使用s,ms等
using namespace std;

class DynamicTfListener: public rclcpp::Node{
private:
    //初始化
    shared_ptr<tf2_ros::TransformListener> broadcastor_;
    rclcpp::TimerBase::SharedPtr timer_;
    shared_ptr<tf2_ros::Buffer> buffer_;
public:
    DynamicTfListener():Node ("dynamic_tf2_listener"){
        //实例化
        this->buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        this ->broadcastor_ = std::make_shared<tf2_ros::TransformListener> (*buffer_,this);
        // this->tf2_transform();
        this->timer_ =create_wall_timer(3s,bind(&DynamicTfListener::Tf_get_Listen,this));
    }

    void Tf_get_Listen(){
    try{
        //查询坐标关系
        const auto msg = buffer_->lookupTransform("base_link","target_point",this->get_clock()->now(),rclcpp::Duration::from_seconds(1));
        auto translation =msg.transform.translation;//获取平移信息     
        auto rotation =msg.transform.rotation;//获取旋转信息
        //将四元数转化为欧拉角
        double y,p,r;//提前定义量传入getEulerYPR
        tf2::getEulerYPR(rotation,y,p,r);
        RCLCPP_INFO(get_logger(),"旋转信息:%f,%f,%f",y,p,r);       
        RCLCPP_INFO(get_logger(),"平移信息:%f,%f,%f",translation.x,translation.y,translation.z);       
    }catch(const exception& e){
        RCLCPP_WARN_STREAM(get_logger(),"找到错误："<<e.what());
    }
        }

    // void tf2_transform(){
    //     auto msg =geometry_msgs::msg::TransformStamped();
    //     msg.header.stamp=this->get_clock()->now();
    //     msg.header.frame_id ="map";
    //     msg.child_frame_id ="base_link";
    //     msg.transform.translation.x=5.0;
    //     msg.transform.translation.y=3.0;
    //     msg.transform.translation.z=1.0;
    //     tf2::Quaternion q;
    //     q.setRPY(0.0,0.0,30*M_PI/180);
    //     msg.transform.rotation = tf2::toMsg(q);
    //     this->broadcastor_->sendTransform(msg);
        
    // }
};

int main(int argc,char** argv){
    rclcpp::init(argc,argv);
    auto node = make_shared<DynamicTfListener>();
    RCLCPP_INFO(node->get_logger(),"启动tf2_listener节点");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}