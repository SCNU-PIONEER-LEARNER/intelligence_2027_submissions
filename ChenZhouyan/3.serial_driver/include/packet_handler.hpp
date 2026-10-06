#ifndef PACKET_HANDLER_HPP
#define PACKET_HANDLER_HPP

#include "packet.hpp"

// 抽象接口：所有“收到一帧数据后要做什么”的共同接口
// 这就是任务说的“找出共同点，抽象出接口”
// virtual 表示这个函数允许子类改写；= 0 表示它是纯虚函数，
// 这个接口本身不能单独用，必须由子类（具体 Handler）来实现。
class IPacketHandler {
 public:
  // 收到一帧数据就处理它（每个 Handler 自己决定怎么处理）
  // override 表示这里确实改写了父类里定义的虚函数。
  virtual void onPacket(const ReceivePacket& packet) = 0;
};

#endif
