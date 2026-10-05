#include <opencv2/opencv.hpp>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
int main(){
    cv::Mat img = cv::imread("/home/rb/resources/test.jpg", cv::IMREAD_COLOR);
    if(img.empty()){
        std::cout << "Image not found!" << std::endl;
        return -1;
    }
    cv::imshow("test_image", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}