#include<iostream>
#include<rclcpp/rclcpp.hpp>
#include<opencv2/opencv.hpp>
int main(){
    cv::Mat image=imread("/home/rb/resources/task_image.png",cv::IMREAD_COLOR);
    cv::Mat gray_image;
    cv::cvtColor(image,gray_image,cv::COLOR_BGR2GRAY);
    cv::Mat binary_image;
    cv::threshold(gray_image,binary_image,0,255,cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::Mat kernal = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::Mat rk1cleaned_image;
    cv::morphologyEx(binary_image,rk1cleaned_image,cv::MORPH_CLOSE,kernal);
    cv::imshow("original",image);
    cv::imshow("gray image",gray_image);
    cv::imshow("binary image",binary_image);
    cv::imshow("rk1cleaned image",rk1cleaned_image);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}