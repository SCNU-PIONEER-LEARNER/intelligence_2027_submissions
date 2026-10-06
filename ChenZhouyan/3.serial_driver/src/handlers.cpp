#include "handlers.hpp"
#include <iostream>
using namespace std;

// 只管“切换识别红/蓝”
void DetectColorHandler::onPacket(const ReceivePacket& packet) {
  if (packet.control.detect_color == 0) {
    cout << "[DetectColor] 切换识别颜色为 红色" << endl;
  } else {
    cout << "[DetectColor] 切换识别颜色为 蓝色" << endl;
  }
}

// 只管“重置追踪器”
void TrackerResetHandler::onPacket(const ReceivePacket& packet) {
  if (packet.control.reset_tracker) {
    cout << "[TrackerReset] 重置追踪器" << endl;
  }
}

// 只管“广播云台姿态”
void GimbalTfHandler::onPacket(const ReceivePacket& packet) {
  cout << "[GimbalTF] 广播云台姿态 roll=" << packet.gimbal.roll
       << " pitch=" << packet.gimbal.pitch
       << " yaw=" << packet.gimbal.yaw << endl;
}

// 只管“发布瞄准点”
void AimingMarkerHandler::onPacket(const ReceivePacket& packet) {
  cout << "[AimingMarker] 发布瞄准点 ("
       << packet.aiming.x << ", "
       << packet.aiming.y << ", "
       << packet.aiming.z << ")" << endl;
}
