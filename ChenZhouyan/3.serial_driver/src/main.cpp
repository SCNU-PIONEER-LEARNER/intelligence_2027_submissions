#include "rm_serial_driver.hpp"
#include "handlers.hpp"
#include <iostream>
using namespace std;

int main() {
  // 造一个驱动器
  RMSerialDriver driver;

  // 造四个 Handler（直接写在 main 里，用 & 取地址注册）
  DetectColorHandler h1;
  TrackerResetHandler h2;
  GimbalTfHandler h3;
  AimingMarkerHandler h4;

  driver.registerHandler(&h1);
  driver.registerHandler(&h2);
  driver.registerHandler(&h3);
  driver.registerHandler(&h4);

  // 造一帧假数据（模拟串口收到的一帧）
  ReceivePacket packet;
  packet.control.detect_color = 0;      // 识别红色
  packet.control.reset_tracker = false;  // 不重置追踪器
  packet.gimbal.roll = 0.10f;
  packet.gimbal.pitch = 0.20f;
  packet.gimbal.yaw = 1.57f;
  packet.aiming.x = 0.50f;
  packet.aiming.y = 0.00f;
  packet.aiming.z = 1.20f;

  // 驱动器把这一帧分发出去，四个 Handler 各处理自己那块
  cout << "===== 收到一帧数据，开始分发 =====" << endl;
  driver.receiveData(packet);
  cout << "===== 分发完成 =====" << endl;

  return 0;
}
