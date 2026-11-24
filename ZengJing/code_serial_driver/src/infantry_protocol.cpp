#include "rm_serial_driver/infantry_protocol.hpp"
#include <cstdint>
#include <vector>
#include <chrono>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
namespace rm_serial_driver{
  InfantryProtocol::InfantryProtocol(rclcpp::Node * node)
  : Protocol(node)
{
   // for debug
    is_open_fire_control = node->declare_parameter("is_open_fire_control", true);
    // TF broadcaster
    timestamp_offset_ = node->declare_parameter("timestamp_offset", 0.0);
    yaw_to_pitch_distance_ = node->declare_parameter("yaw_to_pitch_distance", 0.0);
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*node);
    // Create Publisher
    task_pub_ = node->create_publisher<std_msgs::msg::String>("task_mode", 10);
    latency_pub_ = node->create_publisher<std_msgs::msg::Float64>("latency", 10);
    bullet_speed_pub_ = node->create_publisher<std_msgs::msg::Float32>("bullet_speed", 10);
    record_controller_pub_ = node->create_publisher<std_msgs::msg::String>("record_controller", 10);
    is_play_pub_ = node->create_publisher<std_msgs::msg::Bool>("is_play", 10);
    // Detect parameter clients
    armor_detector_param_client_ =
      std::make_shared<rclcpp::AsyncParametersClient>(node, "armor_detector");
    buff_detector_param_client_ =
      std::make_shared<rclcpp::AsyncParametersClient>(node, "buff_detector");

    // Tracker reset service client
    reset_tracker_client_ = node->create_client<std_srvs::srv::Trigger>("tracker/reset");

    // Target change service cilent
    change_target_client_ = node->create_client<std_srvs::srv::Trigger>("tracker/change");
}
    void InfantryProtocol::onReceivePacket(const ControllerCommand & cmd)
{
  const auto packet_detect_color = static_cast<uint8_t>(cmd.detect_color);
              //先强制转化数据里的颜色，方便后续处理
              //把里面的实现逻辑解耦出来
              //同步颜色的逻辑函数 判断同步处理颜色的逻辑
            if (
              !has_desired_detect_color_.load() ||
              packet_detect_color != desired_detect_color_.load())
              {
                syncDetectColorRequest(packet_detect_color); //同步并检测是否同步成功
              }
            //是否要reset的逻辑，攻击目标转换的逻辑
            handleControllerTaskCmd(cmd);
            //速度和开火这些值的初始化
            publishControllerAuxData(cmd);
            //TF坐标变换和发布
            publishGimbalTf(cmd);
}

void InfantryProtocol::receiveTopic(const std_msgs::msg::UInt8MultiArray::ConstSharedPtr msg)
{
  ///之前处理虚拟下位机发过来的包的时候给他写死了，现在转化成这样再去处理数据包
   codec_.copydata(msg->data.data(), msg->data.size());
   ControllerCommand cmd;
  if(codec_.decode(cmd)) {
    onReceivePacket(cmd);
  }
}
  void InfantryProtocol::syncDetectColorRequest(uint8_t color)
  {

              desired_detect_color_.store(color);
              has_desired_detect_color_.store(true);
              detect_color_param_version_.fetch_add(1);
              armor_detect_color_synced_.store(false);
              buff_detect_color_synced_.store(false);
              //判断上方的逻辑是否同步好了
            if (
              !armor_detect_color_synced_.load() ||
              !buff_detect_color_synced_.load()) {
                /////////////////////这里有一个我发现的严重问题，如果视觉挂了，那么就会一直发送修改变量的请求，一直轰炸服务器，具体怎么优化，我觉得要加一个重复次数的判断，如果短期内一直重复，那就是有问题的
              setParam(rclcpp::Parameter("detect_color", static_cast<int>(color)));
            }

  }
  void InfantryProtocol::handleControllerTaskCmd(const ControllerCommand & cmd)
  {
  if (cmd.reset_tracker) {
              resetTracker();
            }

            if (cmd.change_target) {
              changeTarget();
            }
            //具体就看和电控约定好的那个包，我不管你0，1，2代表什么，我直接关键字索引，这样就能少改很多代码了
            std_msgs::msg::String task;
            if (cmd.task_mode == ControllerCommand::Task::aim)
              task.data = "aim";
            else if (cmd.task_mode == ControllerCommand::Task::small_buff)
              task.data = "small_buff";
            else if (cmd.task_mode == ControllerCommand::Task::large_buff)
              task.data = "large_buff";
            // 保底是自瞄
            else
              task.data = "aim";
            task_pub_->publish(task); //发布给视觉节点
  }
  void InfantryProtocol::publishControllerAuxData(const ControllerCommand & cmd)
  {
    std_msgs::msg::Float32 bullet_speed;
            bullet_speed.data =cmd.bullet_speed;
            bullet_speed_pub_->publish(bullet_speed);
            std_msgs::msg::String record_controller;
            record_controller.data = cmd.is_play ? "start" : "stop";
            record_controller_pub_->publish(record_controller);
            std_msgs::msg::Bool is_play;
            is_play.data = cmd.is_play!=0;
            is_play_pub_->publish(is_play);
  }
  void InfantryProtocol::publishGimbalTf(const ControllerCommand & cmd)
  {
  //后续就是TF坐标的发布了
            geometry_msgs::msg::TransformStamped yaw_t;
            timestamp_offset_ = this->get_parameter("timestamp_offset").as_double();//从yaml文件里面读取时间的延迟，就是从电控单片机发送消息到上位机的延迟
            auto stamp = this->now() - rclcpp::Duration::from_seconds(timestamp_offset_);//ros里面的TIME对象只能减去TIME对象或者Duration对象，不能减去一个double，受制于语法，所以做了一个转换
            const double yaw_to_pitch_distance =
              this->get_parameter("yaw_to_pitch_distance").as_double();
            
            // odom -> yaw_link: yaw axis rotation
            yaw_t.header.stamp = stamp;
            yaw_t.header.frame_id = "odom";
            yaw_t.child_frame_id = "yaw_link";
            const double yaw = cmd.yaw;
            const double roll = cmd.roll;
            yaw_t.transform.translation.x = 0.0;
            yaw_t.transform.translation.y = 0.0;
            yaw_t.transform.translation.z = 0.0;  //x,y,z不做平移，这里的坐标变换只是在做角度旋转
            tf2::Quaternion q_yaw; //四元数对象
            q_yaw.setRPY(roll, 0.0, yaw); //利用内置函数把欧拉角转成四元数对象的四个分量
            yaw_t.transform.rotation = tf2::toMsg(q_yaw); //yaw_t的数据结构里面包含transform，transform包含坐标变换和角度变换，这里只用换角度，tomsg做的是把tf的数据类型转化成ros的数据类型，本质上还是四元数对象


            // yaw_link -> pitch_link: fixed axis offset + pitch axis rotation
            geometry_msgs::msg::TransformStamped pitch_t;
            pitch_t.header.stamp = stamp;
            pitch_t.header.frame_id = "yaw_link";
            pitch_t.child_frame_id = "pitch_link";
            pitch_t.transform.translation.x = -yaw_to_pitch_distance;//这个正负就要看云台那里规定的x轴方向了，可以灵活设置
            pitch_t.transform.translation.y = 0.0;
            pitch_t.transform.translation.z = 0.0;
            tf2::Quaternion q_pitch;
            q_pitch.setRPY(0.0, cmd.pitch, 0.0);
            pitch_t.transform.rotation = tf2::toMsg(q_pitch);
            tf_broadcaster_->sendTransform(yaw_t);
            tf_broadcaster_->sendTransform(pitch_t);
  }
   void InfantryProtocol::setParam(const rclcpp::Parameter & param)
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
          if (this->if_calib) return;  // 如果正在标定，跳过报错。不需要改参数，calibrate.py自动处理
          RCLCPP_WARN_THROTTLE(
            get_logger(), *get_clock(), 1000,
            "%s parameter service not ready, skipping detect_color set", detector_name);
          return;
        }

        if (
          future.valid() &&
          future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
          return;
        }

        RCLCPP_INFO(
          get_logger(), "Setting %s detect_color to %ld...", detector_name, param.as_int());
        future = client->set_parameters(
        {param}, [this, param, detector_name, request_color, request_version, synced](
                  const ResultFuturePtr & results) {
          for (const auto & result : results.get()) {
            if (!result.successful) {
              RCLCPP_ERROR(
                get_logger(), "Failed to set %s detect_color: %s", detector_name,
                result.reason.c_str());
              return;
            }
          }
          RCLCPP_INFO(
            get_logger(), "Successfully set %s detect_color to %ld!", detector_name,
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
  void InfantryProtocol::resetTracker()
  {
    if (!reset_tracker_client_->service_is_ready()) {
      RCLCPP_WARN(get_logger(), "Service not ready, skipping tracker reset");
      return;
    }

    auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
    reset_tracker_client_->async_send_request(request);
    RCLCPP_INFO(get_logger(), "Reset tracker!");
  }
  void InfantryProtocol::changeTarget()
  {
    if (!change_target_client_->service_is_ready()) {
      RCLCPP_WARN(get_logger(), "Service not ready, skipping target change");
      return;
    }

    auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
    change_target_client_->async_send_request(request);
    RCLCPP_INFO(get_logger(), "Change target!");
  }
  void InfantryProtocol::sendArmorData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
  {
      auto_aim_interfaces::msg::GimbalCmd cmd = *msg;
      if (!is_open_fire_control)
        cmd.fire = 1;
      //我把处理数据的逻辑放到了packet_codec里面，这样子我认为算是解耦了吧  
      // 20240329 ZY: Eliminate communication latency
                /////////默认给他设置成false，那样encode里面的逻辑就是处理armor的
      return codec_.encode(cmd,false);
  }
//两个函数都是属于协议层的，步兵协议只做解码就行了，这些检测异常和捕获异常就放到驱动层去做就好了，这样维护起来更简单那，也更加解耦
  void InfantryProtocol::sendBuffData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
  {
    return codec_.encode(*msg,true);
  }
}//namespace rm_serial_driver