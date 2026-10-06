// SPDX-License-Identifier: Apache-2.0
#ifndef RM_SERIAL_DRIVER__RECEIVE_DATA_HPP_
#define RM_SERIAL_DRIVER__RECEIVE_DATA_HPP_

#include <cstdint>

namespace rm_serial_driver
{
// Application data: no wire layout, CRC, ROS message, or serial-port dependency.
struct ControlData
{
  uint8_t detect_color = 0;
  bool reset_tracker = false;
  bool change_target = false;
};

struct StatusData
{
  uint8_t task_mode = 0;
  float bullet_speed = 0.0F;
  bool is_play = false;
};

struct GimbalData
{
  float roll = 0.0F;
  float pitch = 0.0F;
  float yaw = 0.0F;
};

struct ReceivedData
{
  ControlData control;
  StatusData status;
  GimbalData gimbal;
};
}  // namespace rm_serial_driver

#endif  // RM_SERIAL_DRIVER__RECEIVE_DATA_HPP_
