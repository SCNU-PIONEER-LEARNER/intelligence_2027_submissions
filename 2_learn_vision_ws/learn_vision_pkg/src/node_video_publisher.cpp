#include<rclcpp/rclcpp.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cv_bridge/cv_bridge.h>
#include<chrono>
#include<sensor_msgs/msg/image.hpp>
#include<std_msgs/msg/header.hpp>
class video_publisher :public rclcpp::Node{
    public:
        video_publisher():Node("video_publisher"){
            video_publisher_=this->create_publisher<sensor_msgs::msg::Image>("video_frames",10);
            cap_.open("/home/rb/resources/task_video.mp4");
            if(!cap_.isOpened()){
                RCLCPP_ERROR(this->get_logger(),"video not found,check ur directory!");
                return;
            }
            timer_ = this->create_wall_timer(std::chrono::milliseconds(33),std::bind(&video_publisher::timer_callback,this));
            RCLCPP_INFO(this->get_logger(),"video publisher activated");
        }
    private:
        void timer_callback(){
            cv::Mat frame;
            cap_ >> frame;
            if(frame.empty()){
                RCLCPP_INFO(this->get_logger(),"video ended");
                cap_.release();
                timer_->cancel();
                return;
            }
            auto vmsg = cv_bridge::CvImage(std_msgs::msg::Header(),"bgr8",frame).toImageMsg();
            video_publisher_->publish(*vmsg);
        }
        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr video_publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        cv::VideoCapture cap_;
};
int main(int argc,char** argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<video_publisher>());
    rclcpp::shutdown();
    return 0;
}