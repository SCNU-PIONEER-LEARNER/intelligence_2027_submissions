#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>
#include <vector>

class NodeTaskPcd : public rclcpp::Node {
public:
    NodeTaskPcd() : Node("node_task_pcd") {
        // ★ 打开本地视频（改成你自己的路径）
        cap_.open("/home/rb/resources/task_video.mp4");
        if (!cap_.isOpened()) {
            RCLCPP_ERROR(this->get_logger(), "视频文件打开失败，请检查路径！");
            return;
        }

        // 定时器：每 33ms 处理一帧（约 30 FPS）
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(33),
            std::bind(&NodeTaskPcd::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "装甲板识别节点已启动，按 'q' 退出。");
    }

private:
    void timer_callback() {
        cv::Mat frame;
        cap_ >> frame;

        if (frame.empty()) {
            RCLCPP_INFO(this->get_logger(), "视频播放结束。");
            cap_.release();
            timer_->cancel();
            cv::destroyAllWindows();
            rclcpp::shutdown();
            return;
        }

        // ================= 图像预处理 =================
        cv::Mat gray, binary, cleaned;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::morphologyEx(binary, cleaned, cv::MORPH_OPEN, kernel);

        // ================= 寻找轮廓 =================
        cv::Mat contour_input = cleaned.clone();
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(contour_input, contours, hierarchy,
                         cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // ================= 遍历轮廓，筛选装甲板 =================
        for (size_t i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours[i]);
            if (area < 5000) continue;

            cv::RotatedRect rtt_rect = cv::minAreaRect(contours[i]);

            // 宽高比过滤：装甲板接近正方形
            float w = rtt_rect.size.width;
            float h = rtt_rect.size.height;
            float ratio = (w > h) ? (w / h) : (h / w);
            if (ratio > 1.8) continue;

            // 提取四顶点 + 极坐标排序
            cv::Point2f vertices[4];
            rtt_rect.points(vertices);
            std::vector<cv::Point2f> tgt_vertices(vertices, vertices + 4);

            cv::Point2f center = rtt_rect.center;
            std::sort(tgt_vertices.begin(), tgt_vertices.end(),
                [&center](cv::Point2f a, cv::Point2f b) {
                    double angleA = std::atan2(a.y - center.y, a.x - center.x);
                    double angleB = std::atan2(b.y - center.y, b.x - center.x);
                    if (angleA < 0) angleA += 2 * M_PI;
                    if (angleB < 0) angleB += 2 * M_PI;
                    return angleA < angleB;
                });
            // 循环移位对齐到 [左上, 右上, 右下, 左下]
            std::rotate(tgt_vertices.begin(), tgt_vertices.begin() + 2, tgt_vertices.end());

            // 透视变换到 200x200
            cv::Point2f tfd_vertices[4];
            tfd_vertices[0] = cv::Point2f(0, 0);
            tfd_vertices[1] = cv::Point2f(200, 0);
            tfd_vertices[2] = cv::Point2f(200, 200);
            tfd_vertices[3] = cv::Point2f(0, 200);
            cv::Mat transform = cv::getPerspectiveTransform(tgt_vertices.data(), tfd_vertices);
            cv::Mat warped;
            cv::warpPerspective(frame, warped, transform, cv::Size(200, 200));

            // ============ 数字识别（占位符）============
            int digit = -1; // 等 ONNX 模型接入后替换

            // ============ 在原图上绘制结果 ============
            for (int j = 0; j < 4; j++) {
                cv::line(frame, tgt_vertices[j], tgt_vertices[(j+1) % 4],
                         cv::Scalar(0, 255, 0), 2);
            }

            std::string label = (digit >= 0) ? std::to_string(digit) : "TGT";
            cv::Point text_pos(center.x - 20, center.y + 20);
            cv::putText(frame, label, text_pos,
                        cv::FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(0, 0, 255), 3);

            // 左上角贴一个摆正后的小图，方便调试
            cv::Mat small_warped;
            cv::resize(warped, small_warped, cv::Size(100, 100));
            small_warped.copyTo(frame(cv::Rect(10, 10, 100, 100)));
        }

        // ================= 显示结果 =================
        cv::imshow("Armor Detector", frame);

        // 按 'q' 或 ESC 退出
        int key = cv::waitKey(1);
        if (key == 'q' || key == 27) {
            RCLCPP_INFO(this->get_logger(), "用户手动退出。");
            cap_.release();
            timer_->cancel();
            cv::destroyAllWindows();
            rclcpp::shutdown();
        }
    }

    cv::VideoCapture cap_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<NodeTaskPcd>());
    rclcpp::shutdown();
    return 0;
}