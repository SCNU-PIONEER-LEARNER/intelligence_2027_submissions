#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(){
    VideoCapture cap("video/task.mp4");

    //检验是否读取视频
    if (!cap.isOpened()){
        cerr << "无法打开视频" << endl; //cerr错误信息流
        return -1;
    }

    //c++的图像矩阵类Mat
    Mat frame,gray,binary,out;

    //读取并处理
    while(true){
        cap>>frame; //读取帧，赋给frame
        if (frame.empty())break;

        out = frame.clone();

        //灰度化-二值化
        cvtColor(frame,gray,COLOR_BGR2GRAY);    
        GaussianBlur(gray,gray,Size(5,5),0);    //高斯模糊
        threshold(gray,binary,0,255,THRESH_BINARY_INV + THRESH_OTSU);   //返回binary

        //形态学闭运算
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
        morphologyEx(binary,binary,MORPH_CLOSE,kernel);

        //轮廓
        vector<vector<Point>> contours; //Points是opencv里的坐标函数类型。一条轮廓上的所有点放在内层vector，所有轮廓放在外层vector。
        findContours(binary,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

        //绘制轮廓
        for (const auto& c: contours){
            //cout<<c<<endl;
            if(contourArea(c)<15000)continue;

            //轮廓线
            vector<vector<Point>> tmp = {c};
            drawContours(out,tmp,-1,Scalar(0,255,0),1); //BGR

            //外接矩形：得到矩形->转化成4个点坐标
            RotatedRect rect = minAreaRect(c);  //RotatedRect旋转矩形
            Point2f pts[4];     //Points2f  是放二维浮点坐标点的数组,pts[i].x pts[i].y
            rect.points(pts);   //将rect转化成4个点的数据

            //绘制矩形
            for (int i = 0;i<4;i++){
                //划线函数line(image,起点，终点，颜色，线宽)；
                line(out,pts[i],pts[(i+1)%4],Scalar(0,0,255),2);
            }
            //绘制中心点
            circle(out,rect.center,5,Scalar(0,0,255),-1);    //-1为填充满

        }

        imshow("outline",out);
        if(waitKey(33)==27)break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}