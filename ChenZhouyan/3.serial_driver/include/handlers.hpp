#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include "packet_handler.hpp"

// 四个 Handler，每个只负责一件事（单一职责）
class DetectColorHandler : public IPacketHandler {
 public:
  void onPacket(const ReceivePacket& packet) override;
};

class TrackerResetHandler : public IPacketHandler {
 public:
  void onPacket(const ReceivePacket& packet) override;
};

class GimbalTfHandler : public IPacketHandler {
 public:
  void onPacket(const ReceivePacket& packet) override;
};

class AimingMarkerHandler : public IPacketHandler {
 public:
  void onPacket(const ReceivePacket& packet) override;
};

#endif
