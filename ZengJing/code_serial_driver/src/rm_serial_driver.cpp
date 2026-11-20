  // Copyright (c) 2022 ChenJun
  // Licensed under the Apache-2.0 License.

  #include "rm_serial_driver/rm_serial_driver.hpp"

  #include <tf2/LinearMath/Quaternion.h>
  #include <cmath>
  #include <cstdint> //给出约定的数据类型是多少字节或者比特
  #include <functional>
  #include <map>
  #include <memory>
  #include <string>
  #include <vector>
  #include <rclcpp/logging.hpp>
  #include <rclcpp/qos.hpp>
  #include <rclcpp/utilities.hpp>
  #include "rm_serial_driver/crc.hpp"
  #include "rm_serial_driver/packet.hpp"
  #include <serial_driver/serial_driver.hpp>
  #include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

  namespace rm_serial_driver
  {

  RMSerialDriver::RMSerialDriver(const rclcpp::NodeOptions & options) //构造函数，给Node初始化名字还有一些基本的配置，主要是设置进程内通信开关ues_intra_process_comms
  : Node("rm_serial_driver", options),
    owned_ctx_{new IoContext(2)},
    serial_driver_{new drivers::serial_driver::SerialDriver(*owned_ctx_)}
  {
    RCLCPP_INFO(get_logger(), "Start RMSerialDriver!");
    use_virtual_board_=this->declare_parameter("use_virtual_board",false);
    packet_topic_=this->declare_parameter("packet_topic",std::string("/receive_packet"));
    ////////////////////////////////////////////////////////////////////////////////////这个getparams只有是真串口才能运，虚拟下位机直接就报错了
    if(!use_virtual_board_)
    {
      getParams();
    }
    // for debug
    is_open_fire_control = this->declare_parameter("is_open_fire_control", true);
    // TF broadcaster
    timestamp_offset_ = this->declare_parameter("timestamp_offset", 0.0);
    yaw_to_pitch_distance_ = this->declare_parameter("yaw_to_pitch_distance", 0.0);
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    // Create Publisher
    task_pub_ = this->create_publisher<std_msgs::msg::String>("task_mode", 10);
    latency_pub_ = this->create_publisher<std_msgs::msg::Float64>("latency", 10);
    bullet_speed_pub_ = this->create_publisher<std_msgs::msg::Float32>("bullet_speed", 10);
    record_controller_pub_ = this->create_publisher<std_msgs::msg::String>("record_controller", 10);
    is_play_pub_ = this->create_publisher<std_msgs::msg::Bool>("is_play", 10);
    // marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("/aiming_point", 10);
    aim_time_info_pub_ =
      this->create_publisher<auto_aim_interfaces::msg::TimeInfo>("time_info/aim", 10);

    // Detect parameter clients
    armor_detector_param_client_ =
      std::make_shared<rclcpp::AsyncParametersClient>(this, "armor_detector");
    buff_detector_param_client_ =
      std::make_shared<rclcpp::AsyncParametersClient>(this, "buff_detector");

    // Tracker reset service client
    reset_tracker_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/reset");

    // Target change service cilent
    change_target_client_ = this->create_client<std_srvs::srv::Trigger>("tracker/change");
    if(use_virtual_board_)
    {
       RCLCPP_INFO(get_logger(), "virtual board mode: subscribe '%s'", packet_topic_.c_str());
       packet_sub_ = this->create_subscription<std_msgs::msg::UInt8MultiArray>(
      packet_topic_, rclcpp::QoS(10).reliable(),
      std::bind(&RMSerialDriver::receiveTopic, this, std::placeholders::_1));
    }else{
      try {
      serial_driver_->init_port(device_name_, *device_config_);
      if (!serial_driver_->port()->is_open()) {
        serial_driver_->port()->open();
        receive_thread_ = std::thread(&RMSerialDriver::receiveData, this);
      }
    } catch (const std::exception & ex) {
      RCLCPP_ERROR(
        get_logger(), "Error creating serial port: %s - %s", device_name_.c_str(), ex.what());
      throw ex;
    }
    }
    

    // Create Subscription
    aim_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalCmd>(
      "armor_planner/cmd_gimbal", rclcpp::SensorDataQoS(),
      std::bind(&RMSerialDriver::sendArmorData, this, std::placeholders::_1));
    buff_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalCmd>(
      "buff_tracker/cmd_gimbal", rclcpp::SensorDataQoS(),
      std::bind(&RMSerialDriver::sendBuffData, this, std::placeholders::_1));
    heartbeat_ = rm_tools::HeartBeatPublisher::create(this);
  }
  RMSerialDriver::~RMSerialDriver()
  {
    if (receive_thread_.joinable()) {
      receive_thread_.join();
    }

    if (serial_driver_->port()->is_open()) {
      serial_driver_->port()->close();
    }

    if (owned_ctx_) {
      owned_ctx_->waitForExit();
    }
  }

  void RMSerialDriver::receiveData()
  {
    std::vector<uint8_t> header(1); //那个连续字符串，准备给电控传过来的字符串转成结构体
    //把原本给data分配固定内存那一部分给删掉了
    //我把验证crc16的处理逻辑新开了函数放进去了
    while (rclcpp::ok()) {
      try {
        serial_driver_->port()->receive(header); //拿到第三方库里port函数返回的串口底层对象，然后调用这个对象的receive方法
        codec_.copydata(header.data(), header.size());
        ControllerCommand cmd;
        if(codec_.decode(cmd))
        {
          onReceivePacket(cmd);
        }
        
      } catch (const std::exception & ex) {
        RCLCPP_ERROR_THROTTLE( //高阶版的日志打印，比INFO和ERROR好一点，因为可以控制日志的打印时间
          get_logger(), *get_clock(), 20, "Error while receiving data: %s", ex.what());
        reopenPort(); //关闭之前打开的串口，重新尝试打开
      }
    }
  }
  void RMSerialDriver::onReceivePacket(const ControllerCommand & cmd)
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

void RMSerialDriver::receiveTopic(const std_msgs::msg::UInt8MultiArray::ConstSharedPtr msg)
{
  ///之前处理虚拟下位机发过来的包的时候给他写死了，现在转化成这样再去处理数据包
   codec_.copydata(msg->data.data(), msg->data.size());
   ControllerCommand cmd;
  if(codec_.decode(cmd)) {
    onReceivePacket(cmd);
  }
}
  void RMSerialDriver::syncDetectColorRequest(uint8_t color)
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
  void RMSerialDriver::handleControllerTaskCmd(const ControllerCommand & cmd)
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
  void RMSerialDriver::publishControllerAuxData(const ControllerCommand & cmd)
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
  void RMSerialDriver::publishGimbalTf(const ControllerCommand & cmd)
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

  void RMSerialDriver::sendArmorData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
  {
    try {
      auto_aim_interfaces::msg::GimbalCmd cmd = *msg;
      if (!is_open_fire_control)
        cmd.fire = 1;
      //我把处理数据的逻辑放到了packet_codec里面，这样子我认为算是解耦了吧  
      // 20240329 ZY: Eliminate communication latency
      
                /////////默认给他设置成false，那样encode里面的逻辑就是处理armor的
      serial_driver_->port()->send(codec_.encode(cmd,false));
      if (msg->control) {
        std_msgs::msg::Float64 latency;
        latency.data = (this->now() - msg->header.stamp).seconds() * 1000.0;
        RCLCPP_DEBUG_STREAM(get_logger(), "Total latency: " + std::to_string(latency.data) + "ms");
        latency_pub_->publish(latency);
      }
    } catch (const std::exception & ex) {
      RCLCPP_ERROR(get_logger(), "Error while sending data: %s", ex.what());
      reopenPort();
    }
  }

  void RMSerialDriver::sendBuffData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
  {
    try {
      //这里的处理逻辑本质上和上面还是一样的，所以直接利用19期的视频里面那个给他抽象出来就行了
      serial_driver_->port()->send(codec_.encode(*msg,true));
      //默认值设置成true，说明此时在处理能量机关的数据
      if (msg->control) {
        std_msgs::msg::Float64 latency;
        latency.data = (this->now() - msg->header.stamp).seconds() * 1000.0;
        RCLCPP_DEBUG_STREAM(get_logger(), "Total latency: " + std::to_string(latency.data) + "ms");
        latency_pub_->publish(latency);
      }
    } catch (const std::exception & ex) {
      RCLCPP_ERROR(get_logger(), "Error while sending data: %s", ex.what());
      reopenPort();
    }
  }
//getparms基本上不用改，本来就是一个作为初始化的函数，等需求变了再改吧
  void RMSerialDriver::getParams()
  {
    using FlowControl = drivers::serial_driver::FlowControl;
    using Parity = drivers::serial_driver::Parity;
    using StopBits = drivers::serial_driver::StopBits;

    uint32_t baud_rate{};
    auto fc = FlowControl::NONE;
    auto pt = Parity::NONE;
    auto sb = StopBits::ONE;

    try {
      device_name_ = declare_parameter<std::string>("device_name", "");
    } catch (rclcpp::ParameterTypeException & ex) {
      RCLCPP_ERROR(get_logger(), "The device name provided was invalid");
      throw ex;
    }

    try {
      baud_rate = declare_parameter<int>("baud_rate", 0);
    } catch (rclcpp::ParameterTypeException & ex) {
      RCLCPP_ERROR(get_logger(), "The baud_rate provided was invalid");
      throw ex;
    }

    try {
      const auto fc_string = declare_parameter<std::string>("flow_control", "");

      if (fc_string == "none") {
        fc = FlowControl::NONE;
      } else if (fc_string == "hardware") {
        fc = FlowControl::HARDWARE;
      } else if (fc_string == "software") {
        fc = FlowControl::SOFTWARE;
      } else {
        throw std::invalid_argument{
          "The flow_control parameter must be one of: none, software, or "
          "hardware."};
      }
    } catch (rclcpp::ParameterTypeException & ex) {
      RCLCPP_ERROR(get_logger(), "The flow_control provided was invalid");
      throw ex;
    }

    try {
      const auto pt_string = declare_parameter<std::string>("parity", "");

      if (pt_string == "none") {
        pt = Parity::NONE;
      } else if (pt_string == "odd") {
        pt = Parity::ODD;
      } else if (pt_string == "even") {
        pt = Parity::EVEN;
      } else {
        throw std::invalid_argument{"The parity parameter must be one of: none, odd, or even."};
      }
    } catch (rclcpp::ParameterTypeException & ex) {
      RCLCPP_ERROR(get_logger(), "The parity provided was invalid");
      throw ex;
    }

    try {
      const auto sb_string = declare_parameter<std::string>("stop_bits", "");

      if (sb_string == "1" || sb_string == "1.0") {
        sb = StopBits::ONE;
      } else if (sb_string == "1.5") {
        sb = StopBits::ONE_POINT_FIVE;
      } else if (sb_string == "2" || sb_string == "2.0") {
        sb = StopBits::TWO;
      } else {
        throw std::invalid_argument{"The stop_bits parameter must be one of: 1, 1.5, or 2."};
      }
    } catch (rclcpp::ParameterTypeException & ex) {
      RCLCPP_ERROR(get_logger(), "The stop_bits provided was invalid");
      throw ex;
    }

    // 参数表不会存在这个参数。使用calibrate.py启动是为true，其它情况为默认值false
    try {
      this->if_calib = this->declare_parameter<bool>("if_calib", false);
    } catch (rclcpp::ParameterTypeException & ex) {
      RCLCPP_ERROR(this->get_logger(), "The varience \"if_calib\" was invalid");
      throw ex;
    }

    device_config_ =
      std::make_unique<drivers::serial_driver::SerialPortConfig>(baud_rate, fc, pt, sb);
  }
  //接受数据的reopenport是在这里定义的
  void RMSerialDriver::reopenPort()
  {
    RCLCPP_WARN(get_logger(), "Attempting to reopen port");
    try {
      if (serial_driver_->port()->is_open()) {
        serial_driver_->port()->close();
      }
      serial_driver_->port()->open();
      RCLCPP_INFO(get_logger(), "Successfully reopened port");
    } catch (const std::exception & ex) {
      RCLCPP_ERROR(get_logger(), "Error while reopening port: %s", ex.what());
      if (rclcpp::ok()) {
        rclcpp::sleep_for(std::chrono::seconds(1));
        reopenPort();
      }
    }
  }

  void RMSerialDriver::setParam(const rclcpp::Parameter & param)
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

  void RMSerialDriver::resetTracker()
  {
    if (!reset_tracker_client_->service_is_ready()) {
      RCLCPP_WARN(get_logger(), "Service not ready, skipping tracker reset");
      return;
    }

    auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
    reset_tracker_client_->async_send_request(request);
    RCLCPP_INFO(get_logger(), "Reset tracker!");
  }

  void RMSerialDriver::changeTarget()
  {
    if (!change_target_client_->service_is_ready()) {
      RCLCPP_WARN(get_logger(), "Service not ready, skipping target change");
      return;
    }

    auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
    change_target_client_->async_send_request(request);
    RCLCPP_INFO(get_logger(), "Change target!");
  }

  }  // namespace rm_serial_driver

  #include "rclcpp_components/register_node_macro.hpp"

  // Register the component with class_loader.
  // This acts as a sort of entry point, allowing the component to be discoverable
  // when its library is being loaded into a running process.
  RCLCPP_COMPONENTS_REGISTER_NODE(rm_serial_driver::RMSerialDriver)
