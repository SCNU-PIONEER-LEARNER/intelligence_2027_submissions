//像素坐标系    ：二维，原点在屏幕左上角。轴：u向右，v向下
//图像物理坐标系 ：二维，原点为光轴与像平面的交点（cx，cy）。单位：mm等
//相机坐标系    ：三维，原点为相机光心。X向右，Y向下，Z向前
//世界坐标系    ：三维，自己定义

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

const double FX = 800.0,FY=800.0 ;   //相机焦距
const double CX = 640.0 , CY = 360.0;    //图像中心点
const double width = 140.0;   //灯带数据
const double height = 117.0;

int main(){
    VideoCapture cap("video/task.mp4");
    if(!cap.isOpened()){
        cerr<<"视频读取失败"<<endl;
        return -1;
    }

    //建立世界坐标系,装甲板中心为原点，z轴朝外
    //必须和下边2D的顺序严格对应
    vector<Point3f> objectPoints={  //Point3f:三维点类型，float
        //顺序：左上开始顺时针。z坐标填0
        Point3f(-width/2,height/2,0),
        Point3f(width/2,height/2,0),
        Point3f(width/2,-height/2,0),
        Point3f(-width/2,-height/2,0)
    };  

    // 准备相机内参矩阵和畸变系数
    Mat cameraMatrix = (Mat_<double>(3, 3) << FX, 0, CX, 0, FY, CY, 0, 0, 1);
    Mat distCoeffs = Mat::zeros(1, 5, CV_64F); // 先假设没有镜头畸变

    //读帧+识别灯带
    Mat frame,hsv,mask1,mask2,mask;
    while(true){
        cap>>frame;
        if(frame.empty())break;
        Mat out=frame.clone();
        

        cvtColor(frame,hsv,COLOR_BGR2HSV);
        inRange(hsv,Scalar(0,100,100),Scalar(10,255,255),mask1);
        inRange(hsv,Scalar(170,100,100),Scalar(180,255,255),mask2);
        mask = mask1 | mask2;

        vector<vector<Point>> contours;
        findContours(mask,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

        vector<RotatedRect> bars;
        for(const auto& c : contours){
            //cout<<contourArea(c)<<endl;
            if(contourArea(c)<3000 || contourArea(c)>9000) continue;
            
            RotatedRect rect = minAreaRect(c);

            bars.emplace_back(rect);
        }

        //PnP计算:先排序并选出目标点,再用slovePnP()
        if(bars.size()==2){
            //区分左右灯带
            RotatedRect leftbar = bars[0].center.x < bars[1].center.x ? bars[0] : bars[1];
            RotatedRect rightbar = bars[0].center.x > bars[1].center.x ? bars[0] : bars[1];

            //获取角点并排序：顺序一定要与objectpoints严格相同！
            Point2f pts_left[4],pts_right[4];
            leftbar.points(pts_left);
            rightbar.points(pts_right);

            Point2f l_lu, l_ru, l_ld, l_rd;
            for (Point2f c : pts_left) {
                if (c.x < leftbar.center.x && c.y < leftbar.center.y)l_lu = c;
                if (c.x > leftbar.center.x && c.y < leftbar.center.y)l_ru = c;
                if (c.x < leftbar.center.x && c.y > leftbar.center.y)l_ld = c;
                if (c.x > leftbar.center.x && c.y > leftbar.center.y)l_rd = c; 
            }
            Point2f r_lu, r_ru, r_ld, r_rd;
            for (Point2f c : pts_right) {
                if (c.x < rightbar.center.x && c.y < rightbar.center.y)r_lu = c;
                if (c.x > rightbar.center.x && c.y < rightbar.center.y)r_ru = c;
                if (c.x < rightbar.center.x && c.y > rightbar.center.y)r_ld = c;
                if (c.x > rightbar.center.x && c.y > rightbar.center.y)r_rd = c; 
            }
            
            //4个角点再图像像素坐标系的位置
            vector<Point2f> imagePoints={
                //左上,右上,右下,左下
                l_lu,r_ru,r_rd,l_ld
            };

            

            Mat rvec,tvec;  //revc旋转向量，tvec平移向量
            bool success = solvePnP(objectPoints, imagePoints,cameraMatrix,distCoeffs,rvec,tvec);
            //success返回PnP是否解算成功，
            if(success){
                double distance = norm(tvec);

                //标注
                for(Point2f p:imagePoints){
                 circle(out,p,5,Scalar(0,255,0),-1);
                }  
                string text = format("Distance:%.0f mm",distance);  //转化为string
                putText(out,text,Point(50,80),FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),2);  //putText(图像，内容，字体，缩放，颜色，粗细)

                cout<<"目标距离："<<distance<<"mm"<<endl;
            }
        }


        imshow("out",out);
        if(waitKey(33)==27)break;

    }

    cap.release();
    destroyAllWindows();
    return 0;
}

