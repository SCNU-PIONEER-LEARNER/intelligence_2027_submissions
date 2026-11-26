// 接收报文消费者集合（B1-B7）。
// detectcolor  B1 识别颜色参数同步
// resettracker B2 tracker 复位
// changetarget B3 切换目标
// taskmode     B4 任务模式
// bulletspeed  B5 弹速
// isplay       B6 录制/回放状态
// TF           B7 云台姿态 TF

#include "rm_serial_driver/rm_serial_driver.hpp"
#include "rm_serial_driver/receive_consumer.hpp"
#include <chrono>
#include <utility>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <rcl_interfaces/msg/set_parameters_result.hpp>

namespace rm_serial_driver{

class detectcolor:public ReceiveConsumer{
    public:
    explicit detectcolor(rclcpp::Node* node,bool if_calib_):node_(node),if_calib(if_calib_){
        armor_detector_param_client_ =
    std::make_shared<rclcpp::AsyncParametersClient>(
      node_, "armor_detector");
  buff_detector_param_client_ =
    std::make_shared<rclcpp::AsyncParametersClient>(
      node_, "buff_detector");
    }
    void consume(const ReceivePacket& packet) override {
        const auto packet_detect_color = static_cast<uint8_t>(packet.detect_color);
        if (
            !has_desired_detect_color_.load() ||
            packet_detect_color != desired_detect_color_.load()) {
            desired_detect_color_.store(packet_detect_color);
            has_desired_detect_color_.store(true);
            detect_color_param_version_.fetch_add(1);
            armor_detect_color_synced_.store(false);
            buff_detect_color_synced_.store(false);
          }

          if (
            !armor_detect_color_synced_.load() ||
            !buff_detect_color_synced_.load()) {
            setParam(rclcpp::Parameter("detect_color", static_cast<int>(packet_detect_color)));
          }
    }

    private:
    using ResultFuturePtr = std::shared_future<std::vector<rcl_interfaces::msg::SetParametersResult>>;
    std::atomic_bool has_desired_detect_color_{false};
    std::atomic<uint8_t> desired_detect_color_{0};
    std::atomic_size_t detect_color_param_version_{0};
    std::atomic_bool armor_detect_color_synced_{false};
    std::atomic_bool buff_detect_color_synced_{false};
    rclcpp::AsyncParametersClient::SharedPtr armor_detector_param_client_;
    rclcpp::AsyncParametersClient::SharedPtr buff_detector_param_client_;
    ResultFuturePtr armor_set_param_future_;
    ResultFuturePtr buff_set_param_future_;
    rclcpp::Node * node_;
    bool if_calib;

    void setParam(const rclcpp::Parameter & param)
    {
    const auto request_color = static_cast<uint8_t>(param.as_int());
    const auto request_version = detect_color_param_version_.load();

    auto set_detector_param =
        [this, &param, request_color, request_version](
        const char * detector_name, const rclcpp::AsyncParametersClient::SharedPtr & client,
        ResultFuturePtr & future, std::atomic_bool * synced) {

        if (synced->load()) {
            return;
        }

        if (!client->service_is_ready()) {
            if (if_calib) return;
            auto getclock = node_->get_clock();
            RCLCPP_WARN_THROTTLE(
            node_->get_logger(),*getclock, 1000,
            "%s parameter service not ready, skipping detect_color set", detector_name);
            return;
        }

        if (
            future.valid() &&
            future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            return;
        }

        RCLCPP_INFO(
            node_->get_logger(), "Setting %s detect_color to %ld...", detector_name, param.as_int());

        future = client->set_parameters(
        {param}, [this, param, detector_name, request_color, request_version, synced](
                    const ResultFuturePtr & results) {

            for (const auto & result : results.get()) {
            if (!result.successful) {
                RCLCPP_ERROR(
                node_->get_logger(), "Failed to set %s detect_color: %s", detector_name,
                result.reason.c_str());
                return;
            }
            }
            RCLCPP_INFO(
            node_->get_logger(), "Successfully set %s detect_color to %ld!", detector_name,
            param.as_int());

            if (
            request_version == detect_color_param_version_.load() &&
            request_color == desired_detect_color_.load()) {
            synced->store(true);
            }
        });
        };

    set_detector_param(
        "armor_detector", armor_detector_param_client_, armor_set_param_future_,
        &armor_detect_color_synced_);
    set_detector_param(
        "buff_detector", buff_detector_param_client_, buff_set_param_future_,
        &buff_detect_color_synced_);
    }

};

class resettracker:public ReceiveConsumer{
    public:

    explicit resettracker(rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr reset_tracker_client,rclcpp::Logger logger):reset_tracker_client_(std::move(reset_tracker_client)),logger_(logger){}

    void consume(const ReceivePacket& packet) override {
        if (packet.reset_tracker) {
            if (!reset_tracker_client_->service_is_ready()) {
            RCLCPP_WARN(logger_, "Service not ready, skipping tracker reset");
    return;
  }

  auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
  reset_tracker_client_->async_send_request(request);
  RCLCPP_INFO(logger_, "Reset tracker!");
          }
    }
    private:
    rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr reset_tracker_client_;
    rclcpp::Logger logger_;
};

class changetarget:public ReceiveConsumer{
    public:

    explicit changetarget(rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr change_target_client,rclcpp::Logger logger):change_target_client_(std::move(change_target_client)),logger_(logger){}
    void consume(const ReceivePacket& packet) override {
        if (packet.change_target) {
        if (!change_target_client_->service_is_ready()) {
    RCLCPP_WARN(logger_, "Service not ready, skipping target change");
    return;
  }

  auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
  change_target_client_->async_send_request(request);
  RCLCPP_INFO(logger_, "Change target!");
        }
    }
    private:
    rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr change_target_client_;
    rclcpp::Logger logger_;
};

class taskmode:public ReceiveConsumer{
    public:
    explicit taskmode(rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub):pub_(std::move(pub)){}
    void consume(const ReceivePacket& packet) override {
        std_msgs::msg::String task;
        if (packet.task_mode == 0)
        task.data = "aim";
        else if (packet.task_mode == 1)
        task.data = "small_buff";
        else if (packet.task_mode == 2)
        task.data = "large_buff";

        else
        task.data = "aim";
        pub_->publish(task);
    }
    private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;

};

class bulletspeed:public ReceiveConsumer{
    public:

    explicit bulletspeed (rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub):pub_(std::move(pub)){}
    void consume(const ReceivePacket& packet) override {
        std_msgs::msg::Float32 bullet_speed;
        bullet_speed.data = packet.bullet_speed;
        pub_->publish(bullet_speed);

    }
    private:
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_;

};

class isplay:public ReceiveConsumer{
    public:
    explicit isplay (rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record,rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub): record_(std::move(record)), pub_(std::move(pub)){}
    void consume(const ReceivePacket& packet) override {
        std_msgs::msg::String record_controller;
        record_controller.data = packet.is_play ? "start" : "stop";
        record_->publish(record_controller);
        std_msgs::msg::Bool is_play;
        is_play.data = packet.is_play != 0;
        pub_->publish(is_play);
    }
    private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr record_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_;

};

class TF:public ReceiveConsumer{
    public:

    explicit TF(std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster,rclcpp::Node* node):tf_broadcaster_(std::move(tf_broadcaster)),node_(node){}
    void consume(const ReceivePacket& packet) override {
          geometry_msgs::msg::TransformStamped yaw_t;
          auto timestamp_offset_ = node_->get_parameter("timestamp_offset").as_double();
          auto stamp = node_->now() - rclcpp::Duration::from_seconds(timestamp_offset_);
          const double yaw_to_pitch_distance =
            node_->get_parameter("yaw_to_pitch_distance").as_double();

          yaw_t.header.stamp = stamp;
          yaw_t.header.frame_id = "odom";
          yaw_t.child_frame_id = "yaw_link";
          const double yaw = packet.yaw;
          const double roll = packet.roll;
          yaw_t.transform.translation.x = 0.0;
          yaw_t.transform.translation.y = 0.0;
          yaw_t.transform.translation.z = 0.0;
          tf2::Quaternion q_yaw;

          q_yaw.setRPY(roll, 0.0, yaw);
          yaw_t.transform.rotation = tf2::toMsg(q_yaw);

          geometry_msgs::msg::TransformStamped pitch_t;
          pitch_t.header.stamp = stamp;
          pitch_t.header.frame_id = "yaw_link";
          pitch_t.child_frame_id = "pitch_link";

          pitch_t.transform.translation.x = -yaw_to_pitch_distance;
          pitch_t.transform.translation.y = 0.0;
          pitch_t.transform.translation.z = 0.0;
          tf2::Quaternion q_pitch;
          q_pitch.setRPY(0.0, packet.pitch, 0.0);
          pitch_t.transform.rotation = tf2::toMsg(q_pitch);

          tf_broadcaster_->sendTransform(yaw_t);
          tf_broadcaster_->sendTransform(pitch_t);
    }
    private:
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Node * node_;
};

void RMSerialDriver::registerConsumers(){
  consumers_.push_back(std::make_unique<detectcolor>(this,if_calib));
  consumers_.push_back(std::make_unique<resettracker>(reset_tracker_client_,get_logger()));
  consumers_.push_back(std::make_unique<changetarget>(change_target_client_,get_logger()));
  consumers_.push_back(std::make_unique<taskmode>(task_pub_));
  consumers_.push_back(std::make_unique<bulletspeed>(bullet_speed_pub_));
  consumers_.push_back(std::make_unique<isplay>(record_controller_pub_,is_play_pub_));
  consumers_.push_back(std::make_unique<TF>(std::move(tf_broadcaster_),this));
}

}
