#ifndef RM_SERIAL_DRIVER_HPP
#define RM_SERIAL_DRIVER_HPP

#include "packet.hpp"
#include "packet_handler.hpp"
#include <vector>

// 串口驱动器（上层）：只负责把一帧数据分发给所有 Handler
// 它完全不知道每个 Handler 内部在干什么（分层设计 + 依赖倒置）
class RMSerialDriver {
 public:
  // 注册一个 Handler（用基类指针接收，依赖抽象不依赖细节）
  // 用“父类指针”接收“子类对象”，这就是面向对象里的“多态”：
  // 驱动器只认 IPacketHandler 这个父类，不关心具体是哪个 Handler。
  void registerHandler(IPacketHandler* h) {
    handlers.push_back(h);
  }

  // 收到一帧数据：只做“分发”，具体处理交给各个 Handler
  void receiveData(const ReceivePacket& packet) {
    for (IPacketHandler* h : handlers) {
      h->onPacket(packet);
    }
  }

 private:
  std::vector<IPacketHandler*> handlers;  // 存一堆不同的 Handler
};

#endif
