#include<opencv2/opencv.hpp>
#include<iostream>
#include<rclcpp/rclcpp.hpp>
int main(){
    cv::VideoCapture video("/home/rb/resources/task_video.mp4");
    if(!video.isOpened()){
        std::cout << "Video not found!Check ur directory!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    std::cout << "Press 'q' or 'ESC' to exit the video!" << std::endl;
    bool pause = true;
    while(true){
        if(pause == true){
            video >> frame;
        }
        if(frame.empty()){
            std::cout << "Video ended!" << std::endl;
            break;
        }
        cv::imshow("test_video", frame);
         int key = cv::waitKey(30);
        if(key == 'q' || key == 27){
            std::cout << "Video exited!" << std::endl;
            break;
        }
        if(key == 32){
            pause=!pause;
        }
}
        
    }
    