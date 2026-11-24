#ifndef RM_SERIAL_DRIVER__PROTOCOL_HPP_
#define RM_SERIAL_DRIVER__PROTOCOL_HPP_
#include <cstdint>
#include <vector>
#include <rclcpp/rclcpp.hpp>
#include "auto_aim_interfaces/msg/gimbal_cmd.hpp"
namespace rm_serial_driver
{
     class Protocol
     {
        public:
        Protocol(rclcpp::Node * node):node_(node){};
        virtual void  receiveTopic(const uint8_t * data,size_t length)=0;
        virtual std::vector<uint8_t> sendArmorData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg)=0;
         virtual std::vector<uint8_t> sendBuffData(const auto_aim_interfaces::msg::GimbalCmd::ConstSharedPtr msg) = 0;
        virtual ~Protocol()=default;
        rclcpp::Node * node_;
     };
}
#endif