#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>      // 必须包含，用于 std::atan2 和 M_PI
#include <algorithm>  // 必须包含，用于 std::sort 和 std::rotate

int main() {
    // 1. 读取图像
    cv::Mat image = cv::imread("/home/rb/resources/task_image.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "无法读取图像，请检查路径！" << std::endl;
        return -1;
    }

    // 2. 灰度化与二值化 (使用 INV 让装甲板变白，背景变黑)
    cv::Mat gray_image, binary_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::threshold(gray_image, binary_image, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    // 3. 形态学去噪 (开运算去白点)
    cv::Mat rk1cleaned_image;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(binary_image, rk1cleaned_image, cv::MORPH_OPEN, kernel);

    // 4. 寻找轮廓
    cv::Mat input_contour = rk1cleaned_image.clone();
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(input_contour, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 5. 筛选目标轮廓并提取顶点
    std::vector<cv::Point2f> tgt_vertices;
    cv::RotatedRect target_rect; // ★ 修复1：在循环外声明，避免作用域越界
    bool found = false;

    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        std::cout << "contour " << i << " found, area=" << area << std::endl;

        if (area > 5000) { // 面积过滤
            target_rect = cv::minAreaRect(contours[i]);
            cv::Point2f vertices[4];
            target_rect.points(vertices);
            tgt_vertices.assign(vertices, vertices + 4);
            found = true;
            break; // 找到后直接跳出循环
        }
    }

    if (!found) {
        std::cout << "未找到符合要求的装甲板！" << std::endl;
        return -1;
    }

    // ================= 极坐标排序核心逻辑 =================
    // ★ 修复2：修正拼写错误，正确获取中心点
    cv::Point2f center = target_rect.center; 

    // 使用 Lambda 表达式进行极坐标排序
    std::sort(tgt_vertices.begin(), tgt_vertices.end(), [&center](cv::Point2f a, cv::Point2f b) {
        // 计算相对于中心点的角度 (注意：atan2 的参数顺序是 y在前，x在后)
        double angleA = std::atan2(a.y - center.y, a.x - center.x);
        double angleB = std::atan2(b.y - center.y, b.x - center.x);
        
        // ★ 核心修复：把 -π~π 的负角度，统一映射到 0~2π
        if (angleA < 0) angleA += 2 * M_PI;
        if (angleB < 0) angleB += 2 * M_PI;
        
        return angleA < angleB;
    });

    // ★ 核心修复：排完序后是逆时针 [右下, 左下, 左上, 右上]
    // 通过 std::rotate 循环左移 2 位，变成顺时针 [左上, 右上, 右下, 左下]
    std::rotate(tgt_vertices.begin(), tgt_vertices.begin() + 2, tgt_vertices.end());
    // ======================================================

    // 6. 定义目标点 (顺序必须与上面修正后的 tgt_vertices 一一对应)
    cv::Point2f tfd_vertices[4];
    tfd_vertices[0] = cv::Point2f(0, 0);       // 左上
    tfd_vertices[1] = cv::Point2f(200, 0);     // 右上
    tfd_vertices[2] = cv::Point2f(200, 200);   // 右下
    tfd_vertices[3] = cv::Point2f(0, 200);     // 左下

    // 7. 执行透视变换
    cv::Mat transform = cv::getPerspectiveTransform(tgt_vertices.data(), tfd_vertices);
    cv::Mat warped_image;
    cv::warpPerspective(image, warped_image, transform, cv::Size(200, 200));

    // 8. 显示结果
    cv::imshow("warped image", warped_image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}