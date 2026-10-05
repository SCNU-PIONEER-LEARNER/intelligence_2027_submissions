#include<iostream>
#include<rclcpp/rclcpp.hpp>
#include<sensor_msgs/msg/image.hpp>
#include<cv_bridge/cv_bridge.h>
#include<opencv2/opencv.hpp>

class video_subscriber :public rclcpp::Node{
    public:
    video_subscriber():Node("video_subscriber"){
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>("/video_frames",10,std::bind(&video_subscriber::image_callback,this,std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(),"video subscriber activated");
        
    };
    private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr vmsg){
        try{
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(vmsg,"bgr8");
            cv::Mat image= cv_ptr->image;
            cv::imshow("video player",image);
            cv::waitKey(1);
        }
        catch(const cv_bridge::Exception& e){
            RCLCPP_ERROR(this->get_logger(),"cv_bridge error: %s",e.what());
        }
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};
int main(int argc,char** argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<video_subscriber>());
    rclcpp::shutdown();
    cv::destroyAllWindows();
    return 0;
}