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
    //////////////////////多态协议在这里
    protocol_=std::make_unique<InfantryProtocol>(this);
    // marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("/aiming_point", 10);
    aim_time_info_pub_ =
      this->create_publisher<auto_aim_interfaces::msg::TimeInfo>("time_info/aim", 10);


    
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
        protocol_->receiveTopic(header.data(), header.size());
        //把之前定义的那个结构体相关的代码删掉 
      } catch (const std::exception & ex) {
        RCLCPP_ERROR_THROTTLE( //高阶版的日志打印，比INFO和ERROR好一点，因为可以控制日志的打印时间
          get_logger(), *get_clock(), 20, "Error while receiving data: %s", ex.what());
        reopenPort(); //关闭之前打开的串口，重新尝试打开
      }
    }
  }
  
  void RMSerialDriver::sendArmorData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)
  {
    try {
      // 20240329 ZY: Eliminate communication latency
                ////////处理逻辑封装好了，进入步兵协议之后再调用packet_codec的encode函数，真正的把细节隐藏了起来
      serial_driver_->port()->send(protocol_->sendArmorData(msg));
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
      serial_driver_->port()->send(protocol_->sendArmorData(msg));
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

 

  

  v

  }  // namespace rm_serial_driver

  #include "rclcpp_components/register_node_macro.hpp"

  // Register the component with class_loader.
  // This acts as a sort of entry point, allowing the component to be discoverable
  // when its library is being loaded into a running process.
  RCLCPP_COMPONENTS_REGISTER_NODE(rm_serial_driver::RMSerialDriver)
