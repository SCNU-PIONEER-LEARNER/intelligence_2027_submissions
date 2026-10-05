#include<iostream>
#include<opencv2/opencv.hpp>
#include<rclcpp/rclcpp.hpp>
int main(){
    cv::VideoCapture cap_;
    cap_.open("/home/rb/resources/task_video.mp4");
    if(!cap_.isOpened()){    
        std::cout << "Video not found!Check ur directory!" << std::endl;
        return -1;
    }
    cv::Mat frame;
    std::cout << "Press 'q' or 'ESC' to exit the video!" << std::endl;
    bool pause = true;
    while(true){
        if(pause == true){
            cap_ >> frame;
        }
        if(frame.empty()){
            std::cout << "Video ended!" << std::endl;
            break;
        }
        cv::Mat grayframe;
        cv::Mat output_image=frame.clone();
        cv::cvtColor(frame,grayframe,cv::COLOR_BGR2GRAY);
        cv::Mat binaryframe;
        cv::threshold(grayframe,binaryframe,200,255,cv::THRESH_BINARY);
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
        cv::Mat cleanedframe;
        cv::morphologyEx(binaryframe,cleanedframe,cv::MORPH_CLOSE,kernel);
        int key = cv::waitKey(30);
        std::vector<std::vector<cv::Point>> contours_i;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(cleanedframe,contours_i,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
        for(size_t i=0;i<contours_i.size();i++){
            double area=cv::contourArea(contours_i[i]);
            if(area>0){
                cv::drawContours(output_image,contours_i,int(i),cv::Scalar(0,255,0),2);
                std::cout<<"contour "<<i<<" found,area="<<area<<std::endl;
            }
        }
        cv::imshow("Output Image",output_image);
        if(key == 'q' || key == 27){
            std::cout << "Video exited!" << std::endl;
            break;
        }
        if(key == 32){
            pause=!pause;
        }
    }
}