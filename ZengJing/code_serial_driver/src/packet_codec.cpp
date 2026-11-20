#include "rm_serial_driver/packet_codec.hpp"
#include "rm_serial_driver/packet.hpp"
#include  <cstring>
#include <string>
#include <vector>
namespace rm_serial_driver
{
void PacketCodec::copydata(const uint8_t * data, size_t len)
{
    if (data == nullptr || len == 0) {
        return;
    }
     real_data.insert(real_data.end(), data, data + len);
}
bool PacketCodec::decode(ControllerCommand & out)
{
    //我直接默认receiverpacket是在变化的
    ReceivePacket packet;
    std::memcpy(&packet, real_data.data(), sizeof(packet));
    //只要还是crc校验码在字节流最后，那这个crc.cpp里面定义的检验函数就还能用
    if (!crc16::Verify_CRC16_Check_Sum(
          reinterpret_cast<const uint8_t *>(&packet), sizeof(packet))) {
       RCLCPP_ERROR(get_logger(), "CRC error!");
       return false;
    }
    out.detect_color = static_cast<uint8_t>(packet.detect_color);
    out.task_mode = static_cast<ControllerCommand::Task>(packet.task_mode);
    out.reset_tracker = packet.reset_tracker;
    out.change_target = packet.change_target;
    out.is_play = packet.is_play ;
    out.roll = packet.roll;
    out.pitch = packet.pitch;
    out.yaw = packet.yaw;
    out.bullet_speed = packet.bullet_speed;
    return true;
}
std::vector<uint8_t> PacketCodec::encode(auto_aim_interfaces::msg::GimbalCmd & cmd, bool is_buff)
{
  SendPacket packet;
  packet.control = cmd.control;
  packet.fire = cmd.fire;
  ///把原本的sendbuffdata和sendarmordata直接转化到这里
  if (is_buff) {
    packet.yaw = cmd.yaw;
    packet.pitch = -cmd.pitch;//这个还是之前的问题，具体正负要看相机的坐标轴x方向
    packet.yaw_vel = 0.0;
    packet.yaw_acc = 0.0;
    packet.pitch_vel = 0.0;
    packet.pitch_acc = 0.0;
  } else {
    packet.yaw = cmd.yaw;
    packet.yaw_vel = cmd.yaw_vel;
    packet.yaw_acc = cmd.yaw_acc;
    packet.pitch = cmd.pitch;
    packet.pitch_vel = cmd.pitch_vel;
    packet.pitch_acc = cmd.pitch_acc;
  }
  crc16::Append_CRC16_Check_Sum(reinterpret_cast<uint8_t *>(&packet), sizeof(packet));//给要发送的数据加上crc校验码
  std::vector<uint8_t> frame(sizeof(packet));
  std::memcpy(frame.data(), &packet, sizeof(packet));
  return frame;
}
}