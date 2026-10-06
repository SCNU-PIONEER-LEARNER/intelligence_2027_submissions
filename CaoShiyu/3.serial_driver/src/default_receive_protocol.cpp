// SPDX-License-Identifier: Apache-2.0
#include "rm_serial_driver/default_receive_protocol.hpp"

#include "rm_serial_driver/crc.hpp"
#include "rm_serial_driver/packet.hpp"

namespace rm_serial_driver
{
uint8_t DefaultReceiveProtocol::headerByte() const
{
  return 0x5A;
}

std::size_t DefaultReceiveProtocol::frameSize() const
{
  return sizeof(ReceivePacket);
}

DecodeStatus DefaultReceiveProtocol::decode(
  const std::vector<uint8_t> & bytes, ReceivedData & output) const
{
  // Validate raw bytes before copying or reading any packet fields.
  if (bytes.size() != frameSize()) {
    return DecodeStatus::InvalidSize;
  }
  if (bytes.front() != headerByte()) {
    return DecodeStatus::InvalidHeader;
  }
  if (!crc16::Verify_CRC16_Check_Sum(bytes.data(), static_cast<uint32_t>(bytes.size()))) {
    return DecodeStatus::InvalidChecksum;
  }

  const ReceivePacket packet = fromVector(bytes);
  ReceivedData decoded;
  decoded.control.detect_color = static_cast<uint8_t>(packet.detect_color);
  decoded.control.reset_tracker = packet.reset_tracker;
  decoded.control.change_target = packet.change_target;
  decoded.status.task_mode = static_cast<uint8_t>(packet.task_mode);
  decoded.status.bullet_speed = packet.bullet_speed;
  decoded.status.is_play = packet.is_play != 0;
  decoded.gimbal.roll = packet.roll;
  decoded.gimbal.pitch = packet.pitch;
  decoded.gimbal.yaw = packet.yaw;
  output = decoded;
  return DecodeStatus::Ok;
}
}  // namespace rm_serial_driver
