// ============================================================
// 反例 / 解耦前的 receiveData（节选自开源 rm_serial_driver，已简化）
//
// 任务原文说的问题就在这里：
//   “receiveData 函数承担的职责过多，严重依赖包结构，
//    一旦 ReceivePacket 结构体发生改变，就要在这个函数里
//    大海捞针找对应处理的代码。”
//
// 看下面：一个函数干了 4 件不同的事（颜色 / 重置 / TF / Marker），
// 并且直接依赖 ReceivePacket 的全部字段。
// 以后加第 5、6、7 种处理，全往这个几百行的函数里堆，
// 改一个字段就可能牵一发动全身 —— 典型的“不利于维护”。
// ============================================================
#if 0  // 仅作对照阅读，不参与编译
void RMSerialDriver::receiveData(const ReceivePacket& packet) {
  // 1) 识别颜色
  if (packet.detect_color != last_color_) {
    setParam(Parameter("detect_color", packet.detect_color));
    last_color_ = packet.detect_color;
  }
  // 2) 重置追踪器
  if (packet.reset_tracker) {
    resetTracker();
  }
  // 3) 云台姿态 -> TF 广播
  TransformStamped t;
  t.transform.rotation = toQuaternion(packet.roll, packet.pitch, packet.yaw);
  tf_broadcaster_->sendTransform(t);
  // 4) 瞄准点 -> Marker
  if (abs(packet.aim_x) > 0.01) {
    aiming_point_.pose.position = {packet.aim_x, packet.aim_y, packet.aim_z};
    marker_pub_->publish(aiming_point_);
  }
  // 将来加第5、6、7件事……全堆在这里
}
#endif
