#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main(){
    VideoCapture cap("video/task.mp4");

    if (!cap.isOpened()){
        cerr<<"读取视频错误"<<endl;
        return -1;
    }
    vector<Point2f> center;

    Mat frame,hsv,out,mask1,mask2,mask;
    while(true){
        cap>>frame;
        if(frame.empty())break;
        out = frame.clone();
        center.clear();  // 每帧清空

        //hsv
        cvtColor(frame,hsv,COLOR_BGR2HSV);
        inRange(hsv,Scalar(0,100,100),Scalar(10,255,255),mask1);
        inRange(hsv,Scalar(170,100,100),Scalar(180,255,255),mask2);
        mask = mask1 | mask2;   //mask1和mask2为二值图


        // imshow("mask",mask);
        // waitKey(0);
        // destroyAllWindows();

        //morphology
        Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3));
        morphologyEx(mask,mask,MORPH_OPEN,kernel);
        morphologyEx(mask,mask,MORPH_CLOSE,kernel);
        
        //找轮廓
        vector<vector<Point>> contours;
        findContours(mask,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

        for (const auto& c :contours){
            if(contourArea(c)<1000)continue;

            //画轮廓
            vector<vector<Point>> tmp={c};
            drawContours(out,tmp,-1,Scalar(0,255,0),1);

            //找框
            RotatedRect rect=minAreaRect(c);
            Point2f pts[4];
            rect.points(pts);
            for (int i = 0; i < 4; i++) {
                line(out, pts[i], pts[(i + 1) % 4], Scalar(0, 0, 255), 2);
            }

            center.emplace_back(rect.center);
        }
        //中心点
        if (center.size()>=2){
        Point2f cen = (center[0]+center[1])/2;
        circle(out,cen,5,Scalar(0,0,255),-1);}
    
        imshow("outline",out);
        if(waitKey(33)==27)break;
    }
    cap.release();
    destroyAllWindows();
    return 0;

}