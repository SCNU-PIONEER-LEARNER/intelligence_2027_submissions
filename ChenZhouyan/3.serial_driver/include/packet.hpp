#ifndef PACKET_HPP
#define PACKET_HPP

// 接收数据包：模拟从串口收到的一帧数据
// 把不同用途的数据分成小结构，每个 Handler 只取自己关心的那块

struct GimbalData {   // 云台姿态（单位弧度）
  float roll;
  float pitch;
  float yaw;
};

struct AimingData {   // 瞄准点（世界坐标，单位米）
  float x;
  float y;
  float z;
};

struct ControlData {  // 控制指令
  int detect_color;    // 0=红 1=蓝
  bool reset_tracker;  // true 表示要重置追踪器
};

struct ReceivePacket { // 一帧完整数据
  GimbalData gimbal;
  AimingData aiming;
  ControlData control;
};

#endif
