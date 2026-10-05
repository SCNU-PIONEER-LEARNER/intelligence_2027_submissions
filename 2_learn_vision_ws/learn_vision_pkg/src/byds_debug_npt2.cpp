#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

int main() {
    cv::Mat image = cv::imread("/home/rb/resources/task_image.png", cv::IMREAD_COLOR);
    if (image.empty()) return -1;
    cv::Mat image_out = image.clone(); // 用于画调试图

    // 图像预处理
    cv::Mat gray_image, binary_image, rk1cleaned_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::threshold(gray_image, binary_image, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    cv::Mat kernal = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(binary_image, rk1cleaned_image, cv::MORPH_OPEN, kernal);

    // 找轮廓
    cv::Mat input_contour = rk1cleaned_image.clone();
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(input_contour, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point2f> tgt_vertices;
    cv::RotatedRect rtt_rect;

    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        std::cout << "contour " << i << " found, area=" << area << std::endl;

        if (area > 5000) {
            // ★ 修正：原代码写的是 contours[1]，容易越界，改为 contours[i]
            rtt_rect = cv::minAreaRect(contours[i]); 
            cv::Point2f vertices[4];
            rtt_rect.points(vertices);
            tgt_vertices.assign(vertices, vertices + 4);
            break;
        }
    }

    cv::Point2f center = rtt_rect.center;

    // ================= 原汁原味的极坐标排序（精神污染版） =================
    std::sort(tgt_vertices.begin(), tgt_vertices.end(), [&center](cv::Point2f a, cv::Point2f b) {
        double angleA = std::atan2(a.y - center.y, a.x - center.x);
        double angleB = std::atan2(b.y - center.y, b.x - center.x);
        // 没有负角度修正
        return angleA < angleB;
    });
    // =======================================================================

    // ★ 可视化调试：打印并画出排序后的顶点顺序
    std::cout << "\n========= 精神污染排序结果 =========" << std::endl;
    for (int j = 0; j < 4; j++) {
        // 计算并转换角度到 0~360 度
        double angle = std::atan2(tgt_vertices[j].y - center.y, tgt_vertices[j].x - center.x) * 180.0 / M_PI;
        if (angle < 0) angle += 360.0;
        
        std::cout << "排序后第 " << j << " 个点: (" 
                  << tgt_vertices[j].x << ", " << tgt_vertices[j].y 
                  << "), 角度: " << angle << " 度" << std::endl;

        // 画点（红色实心圆）
        cv::circle(image_out, tgt_vertices[j], 15, cv::Scalar(0, 0, 255), -1);
        // 标数字（白色字体，标 0, 1, 2, 3）
        std::string label = std::to_string(j);
        cv::putText(image_out, label, tgt_vertices[j], cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(255, 255, 255), 3);
        // 连线（蓝色线条，按排序后的顺序连）
        cv::line(image_out, tgt_vertices[j], tgt_vertices[(j+1)%4], cv::Scalar(255, 0, 0), 3);
    }
    std::cout << "===================================\n" << std::endl;

    cv::imshow("Debug Image", image_out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}