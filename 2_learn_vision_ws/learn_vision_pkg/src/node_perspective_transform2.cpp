#include<rclcpp/rclcpp.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cstddef>
#include<vector>
#include<iterator>
#include<cmath>

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
    std::vector<std::vector<cv::Point>> contours_i;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(input_contour,contours_i,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point2f> tgt_vertices;
    cv::RotatedRect rtt_rect;
    for(size_t i=0;i<contours_i.size();i++){
        double area=cv::contourArea(contours_i[i]);
        std::cout<<"contour "<<i<<" found"<<"area="<<area<<std::endl;
        if(area>5000){
            rtt_rect=cv::minAreaRect(contours_i[i]);
            cv::Point2f vertices[4];
            rtt_rect.points(vertices);
            tgt_vertices.assign(vertices,vertices+4);
            break;
        }
    }
    cv::Point2f tfd_vertices[4];
    tfd_vertices[0]=cv::Point2f(0,0);
    tfd_vertices[1]=cv::Point2f(200,0);
    tfd_vertices[2]=cv::Point2f(200,200);
    tfd_vertices[3]=cv::Point2f(0,200);
    cv::Point2f center=rtt_rect.center;
    std::sort(tgt_vertices.begin(),tgt_vertices.end(),[&center](cv::Point2f a,cv::Point2f b){    double angleA = std::atan2(a.y - center.y, a.x - center.x);
    double angleB = std::atan2(b.y - center.y, b.x - center.x);
    return angleA < angleB; });
    cv::Mat transform=cv::getPerspectiveTransform(tgt_vertices.data(),tfd_vertices);
    cv::Mat warped_image;
    cv::warpPerspective(image,warped_image,transform,cv::Size(200,200));
    cv::imshow("warped image",warped_image);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}