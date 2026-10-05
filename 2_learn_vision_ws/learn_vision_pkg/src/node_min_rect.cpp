#include<rclcpp/rclcpp.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cstddef>
#include<vector>

int main(){
    cv::Mat image=imread("/home/rb/resources/task_image.png",cv::IMREAD_COLOR);
    cv::Mat image_out =image;
    cv::Mat gray_image;
    cv::cvtColor(image,gray_image,cv::COLOR_BGR2GRAY);
    cv::Mat binary_image;
    cv::threshold(gray_image,binary_image,0,255,cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    cv::Mat rk1cleaned_image;
    cv::Mat kernal=getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::morphologyEx(binary_image,rk1cleaned_image,cv::MORPH_OPEN,kernal);
    cv::Mat input_contour=rk1cleaned_image.clone();
    cv::Mat contour_image=cv::Mat::zeros(rk1cleaned_image.size(),CV_8UC3);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(input_contour,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    for(size_t i=0;i<contours.size();i++){
        double area=cv::contourArea(contours[i]);
        std::cout<<"contour "<<i<<" found"<<"area="<<area<<std::endl;
        if(area>5000){
            cv::RotatedRect rtt_rect=cv::minAreaRect(contours[i]);
            cv::Point2f vertices[4];
            rtt_rect.points(vertices);
            for(int j=0;j<4;j++){
                cv::line(contour_image,vertices[j],vertices[(j +1)%4],cv::Scalar(0,255,0),2);
                cv::line(image_out,vertices[j],vertices[(j +1)%4],cv::Scalar(0,255,0),2);
            }
            std::cout<<"target location:"<<rtt_rect.center.x<<","<<rtt_rect.center.y<<std::endl;
            std::cout<<"angle:"<<rtt_rect.angle<<std::endl;

        }
    }
    cv::imshow("original",image);
    cv::imshow("gray image",gray_image);
    cv::imshow("binary image",binary_image);
    cv::imshow("rk1cleaned image",rk1cleaned_image);
    cv::imshow("contours",contour_image);
    cv::imshow("image out",image_out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}