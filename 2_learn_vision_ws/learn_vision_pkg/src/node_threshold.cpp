#include<iostream>
#include<rclcpp/rclcpp.hpp>
#include<sensor_msgs/msg/image.hpp>
#include<cv_bridge/cv_bridge.h>
#include<opencv2/opencv.hpp>
class threshold_subscriber :public rclcpp::Node {
    public:
    threshold_subscriber():Node("threshold_subscriber"){
        subscription_=this->create_subscription<sensor_msgs::msg::Image>("/video_frames",10,std::bind(&threshold_subscriber::threshold_callback,this,std::placeholders::_1));
        RCLCPP_INFO(this-> get_logger(),"video thresholding");
    }
    private:
    void threshold_callback(const sensor_msgs::msg::Image::SharedPtr vmsg){
        try {
            cv_bridge::CvImagePtr cv_ptr= cv_bridge::toCvCopy(vmsg,"bgr8");
            cv::Mat image = cv_ptr->image;
            cv::Mat grey_image;
            cv::cvtColor(image,grey_image,cv::COLOR_BGR2GRAY);
            cv::Mat binary_image;
            cv::threshold(grey_image,binary_image,0,255,cv::THRESH_BINARY | cv::THRESH_OTSU);
            cv::imshow("normal image",image);
            cv::imshow("grey image",grey_image);
            cv::imshow("binary image",binary_image);
            cv::waitKey(1);
        }
        catch (const cv_bridge::Exception& e){
            RCLCPP_ERROR(this->get_logger(),"cv_bridge error: '%s'",e.what());
        }
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};
int main(int argc,char** argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<threshold_subscriber>());
    rclcpp::shutdown();
    cv::destroyAllWindows();
    return 0;
}