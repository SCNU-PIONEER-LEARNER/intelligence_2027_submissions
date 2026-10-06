// SPDX-License-Identifier: Apache-2.0
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "rm_serial_driver/crc.hpp"
#include "rm_serial_driver/default_receive_protocol.hpp"
#include "rm_serial_driver/packet.hpp"
#include "rm_serial_driver/receive_pipeline.hpp"

using namespace rm_serial_driver;

namespace
{
void check(bool condition, const char * message)
{
  if (!condition) {
    throw std::runtime_error(message);
  }
}

bool near(float lhs, float rhs)
{
  return std::fabs(lhs - rhs) < 0.00001F;
}

ReceivePacket samplePacket()
{
  ReceivePacket packet{};
  packet.detect_color = 1;
  packet.task_mode = 2;
  packet.reset_tracker = true;
  packet.change_target = true;
  packet.is_play = 1;
  packet.roll = 0.25F;
  packet.pitch = -0.5F;
  packet.yaw = 1.0F;
  packet.bullet_speed = 25.0F;
  return packet;
}

std::vector<uint8_t> encode(const ReceivePacket & packet)
{
  std::vector<uint8_t> bytes(sizeof(packet));
  std::memcpy(bytes.data(), &packet, sizeof(packet));
  crc16::Append_CRC16_Check_Sum(bytes.data(), static_cast<uint32_t>(bytes.size()));
  return bytes;
}

struct ControlSpy final : IControlActions
{
  int sync_calls = 0;
  int reset_calls = 0;
  int change_calls = 0;
  uint8_t color = 0;
  std::vector<std::string> events;

  void syncDetectColor(uint8_t value) override
  {
    ++sync_calls;
    color = value;
    events.push_back("color");
  }
  void resetTracker() override
  {
    ++reset_calls;
    events.push_back("reset");
  }
  void changeTarget() override
  {
    ++change_calls;
    events.push_back("change");
  }
};

struct StatusSpy final : IStatusOutput
{
  int calls = 0;
  StatusData last;
  void publishStatus(const StatusData & data) override
  {
    ++calls;
    last = data;
  }
};

struct GimbalSpy final : IGimbalOutput
{
  int calls = 0;
  GimbalData last;
  void publishGimbal(const GimbalData & data) override
  {
    ++calls;
    last = data;
  }
};

void addStandardHandlers(
  ReceivePipeline & pipeline, ControlSpy & control, StatusSpy & status, GimbalSpy & gimbal)
{
  pipeline.addHandler(std::make_unique<ControlHandler>(control));
  pipeline.addHandler(std::make_unique<StatusHandler>(status));
  pipeline.addHandler(std::make_unique<GimbalHandler>(gimbal));
}

class OrderHandler final : public IReceiveHandler
{
public:
  OrderHandler(std::vector<int> & order, int id) : order_(order), id_(id) {}
  void handle(const ReceivedData &) override { order_.push_back(id_); }

private:
  std::vector<int> & order_;
  int id_;
};

// A second protocol proves that dispatch does not depend on ReceivePacket.
class TestProtocol final : public IReceiveProtocol
{
public:
  uint8_t headerByte() const override { return 0x7E; }
  std::size_t frameSize() const override { return 2; }
  DecodeStatus decode(const std::vector<uint8_t> & bytes, ReceivedData & output) const override
  {
    if (bytes.size() != frameSize()) return DecodeStatus::InvalidSize;
    if (bytes.front() != headerByte()) return DecodeStatus::InvalidHeader;
    ReceivedData decoded;
    decoded.control.detect_color = bytes[1] & 1U;
    decoded.status.task_mode = 1;
    decoded.status.bullet_speed = 18.0F;
    decoded.gimbal.yaw = 0.75F;
    output = decoded;
    return DecodeStatus::Ok;
  }
};

class EmptyProtocol final : public IReceiveProtocol
{
public:
  uint8_t headerByte() const override { return 0; }
  std::size_t frameSize() const override { return 0; }
  DecodeStatus decode(const std::vector<uint8_t> &, ReceivedData &) const override
  {
    return DecodeStatus::InvalidSize;
  }
};

void testValidDecode()
{
  DefaultReceiveProtocol protocol;
  check(protocol.headerByte() == 0x5A, "header changed");
  check(protocol.frameSize() == sizeof(ReceivePacket), "frame size mismatch");
  ReceivedData data;
  check(protocol.decode(encode(samplePacket()), data) == DecodeStatus::Ok, "valid CRC rejected");
  check(data.control.detect_color == 1, "color mismatch");
  check(data.control.reset_tracker && data.control.change_target, "control flags mismatch");
  check(data.status.task_mode == 2 && data.status.is_play, "status flags mismatch");
  check(near(data.status.bullet_speed, 25.0F), "speed mismatch");
  check(near(data.gimbal.roll, 0.25F), "roll mismatch");
  check(near(data.gimbal.pitch, -0.5F), "pitch mismatch");
  check(near(data.gimbal.yaw, 1.0F), "yaw mismatch");
}

void testIndependentWireFixture()
{
  // Original layout: header, 1 flag byte, 4 little-endian IEEE-754 floats, CRC16.
  std::vector<uint8_t> bytes = {
    0x5A, 0x3D, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, 0x00, 0xBF,
    0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0xC8, 0x41, 0x00, 0x00};
  crc16::Append_CRC16_Check_Sum(bytes.data(), static_cast<uint32_t>(bytes.size()));
  ReceivedData data;
  DefaultReceiveProtocol protocol;
  check(protocol.decode(bytes, data) == DecodeStatus::Ok, "wire fixture rejected");
  check(data.control.detect_color == 1, "bit 0 mapping changed");
  check(data.status.task_mode == 2, "bits 1-2 mapping changed");
  check(data.control.reset_tracker && data.control.change_target, "control bits changed");
  check(data.status.is_play, "recording bit changed");
  check(near(data.gimbal.pitch, -0.5F) && near(data.status.bullet_speed, 25.0F), "float layout changed");
}

void testInvalidSizes()
{
  DefaultReceiveProtocol protocol;
  ReceivedData data;
  for (std::size_t size : {std::size_t(0), std::size_t(1), protocol.frameSize() - 1,
      protocol.frameSize() + 1}) {
    check(protocol.decode(std::vector<uint8_t>(size), data) == DecodeStatus::InvalidSize,
      "invalid size accepted");
  }
}

void testInvalidHeader()
{
  DefaultReceiveProtocol protocol;
  ReceivedData data;
  auto bytes = encode(samplePacket());
  bytes[0] = 0xA5;
  crc16::Append_CRC16_Check_Sum(bytes.data(), static_cast<uint32_t>(bytes.size()));
  check(protocol.decode(bytes, data) == DecodeStatus::InvalidHeader, "wrong header accepted");
}

void testCorruptPayload()
{
  DefaultReceiveProtocol protocol;
  ReceivedData data;
  auto bytes = encode(samplePacket());
  bytes[4] ^= 0x01;
  check(protocol.decode(bytes, data) == DecodeStatus::InvalidChecksum, "corrupted payload accepted");
}

void testCorruptChecksum()
{
  DefaultReceiveProtocol protocol;
  ReceivedData data;
  auto bytes = encode(samplePacket());
  bytes.back() ^= 0x80;
  check(protocol.decode(bytes, data) == DecodeStatus::InvalidChecksum, "corrupted CRC accepted");
}

void testRejectedOutputUnchanged()
{
  DefaultReceiveProtocol protocol;
  ReceivedData data;
  check(protocol.decode(encode(samplePacket()), data) == DecodeStatus::Ok, "setup decode failed");
  const auto valid_bytes = encode(samplePacket());
  auto bad_header = valid_bytes;
  bad_header[0] = 0;
  auto bad_crc = valid_bytes;
  bad_crc.back() ^= 1;
  for (const auto & bytes : {std::vector<uint8_t>{}, bad_header, bad_crc}) {
    check(protocol.decode(bytes, data) != DecodeStatus::Ok, "bad frame accepted");
    check(data.control.detect_color == 1 && data.control.reset_tracker &&
      data.control.change_target && data.status.task_mode == 2 && data.status.is_play &&
      near(data.status.bullet_speed, 25.0F) && near(data.gimbal.roll, 0.25F) &&
      near(data.gimbal.pitch, -0.5F) && near(data.gimbal.yaw, 1.0F), "output changed on failure");
  }
}

void testNormalDispatch()
{
  ControlSpy control;
  StatusSpy status;
  GimbalSpy gimbal;
  ReceivePipeline pipeline(std::make_unique<DefaultReceiveProtocol>());
  addStandardHandlers(pipeline, control, status, gimbal);
  check(pipeline.processFrame(encode(samplePacket())) == DecodeStatus::Ok, "dispatch failed");
  check(control.sync_calls == 1 && control.reset_calls == 1 && control.change_calls == 1,
    "control actions missing or duplicated");
  check(control.events == std::vector<std::string>{"color", "reset", "change"},
    "control action order changed");
  check(status.calls == 1 && status.last.task_mode == 2 && status.last.is_play, "status missing");
  check(near(status.last.bullet_speed, 25.0F), "status data lost");
  check(gimbal.calls == 1 && near(gimbal.last.yaw, 1.0F), "gimbal data lost");
}

void testRejectedFramesNotDispatched()
{
  ControlSpy control;
  StatusSpy status;
  GimbalSpy gimbal;
  ReceivePipeline pipeline(std::make_unique<DefaultReceiveProtocol>());
  addStandardHandlers(pipeline, control, status, gimbal);
  auto bad_crc = encode(samplePacket());
  bad_crc.back() ^= 1;
  auto bad_header = encode(samplePacket());
  bad_header[0] = 0;
  for (const auto & bytes : {std::vector<uint8_t>{}, bad_header, bad_crc}) {
    check(pipeline.processFrame(bytes) != DecodeStatus::Ok, "bad frame accepted by pipeline");
  }
  check(control.sync_calls == 0 && control.reset_calls == 0 && control.change_calls == 0 &&
    status.calls == 0 && gimbal.calls == 0, "bad frame triggered side effects");
}

void testFalseFlagsAndRepeatedFrames()
{
  ControlSpy control;
  StatusSpy status;
  GimbalSpy gimbal;
  ReceivePipeline pipeline(std::make_unique<DefaultReceiveProtocol>());
  addStandardHandlers(pipeline, control, status, gimbal);
  auto packet = samplePacket();
  packet.detect_color = 0;
  packet.reset_tracker = false;
  packet.change_target = false;
  packet.is_play = 0;
  packet.task_mode = 0;
  const auto bytes = encode(packet);
  check(pipeline.processFrame(bytes) == DecodeStatus::Ok, "first frame rejected");
  check(pipeline.processFrame(bytes) == DecodeStatus::Ok, "repeated frame rejected");
  check(control.sync_calls == 2 && control.color == 0, "color sync should be retried each frame");
  check(control.reset_calls == 0 && control.change_calls == 0, "false flag triggered service");
  check(status.calls == 2 && !status.last.is_play && status.last.task_mode == 0,
    "false flags or repeated status lost");
  check(gimbal.calls == 2, "repeated gimbal update missing");
}

void testTaskModeNames()
{
  check(std::string(taskModeName(0)) == "aim", "aim mode changed");
  check(std::string(taskModeName(1)) == "small_buff", "small buff mode changed");
  check(std::string(taskModeName(2)) == "large_buff", "large buff mode changed");
  check(std::string(taskModeName(3)) == "aim", "reserved mode fallback changed");
  check(std::string(taskModeName(255)) == "aim", "unknown mode fallback changed");
}

void testRegistrationOrderAndExtension()
{
  std::vector<int> order;
  ReceivePipeline pipeline(std::make_unique<DefaultReceiveProtocol>());
  pipeline.addHandler(std::make_unique<OrderHandler>(order, 1));
  pipeline.addHandler(std::make_unique<OrderHandler>(order, 2));
  pipeline.addHandler(std::make_unique<OrderHandler>(order, 3));
  pipeline.addHandler(std::make_unique<OrderHandler>(order, 4));
  check(pipeline.processFrame(encode(samplePacket())) == DecodeStatus::Ok, "extension failed");
  check(order == std::vector<int>{1, 2, 3, 4}, "handlers called out of order");
}

void testReplaceProtocol()
{
  ControlSpy control;
  StatusSpy status;
  GimbalSpy gimbal;
  ReceivePipeline pipeline(std::make_unique<TestProtocol>());
  addStandardHandlers(pipeline, control, status, gimbal);
  check(pipeline.headerByte() == 0x7E && pipeline.frameSize() == 2, "protocol not replaceable");
  check(pipeline.processFrame({0x7E, 1}) == DecodeStatus::Ok, "alternative protocol failed");
  check(control.color == 1 && status.last.task_mode == 1 &&
    near(status.last.bullet_speed, 18.0F) && near(gimbal.last.yaw, 0.75F),
    "alternative protocol cannot reuse handlers");
}

void testInvalidConfiguration()
{
  bool null_protocol = false;
  bool empty_protocol = false;
  bool null_handler = false;
  try {
    ReceivePipeline pipeline(nullptr);
  } catch (const std::invalid_argument &) {
    null_protocol = true;
  }
  try {
    ReceivePipeline pipeline(std::make_unique<EmptyProtocol>());
  } catch (const std::invalid_argument &) {
    empty_protocol = true;
  }
  try {
    ReceivePipeline pipeline(std::make_unique<DefaultReceiveProtocol>());
    pipeline.addHandler(nullptr);
  } catch (const std::invalid_argument &) {
    null_handler = true;
  }
  check(null_protocol && empty_protocol && null_handler, "invalid configuration accepted");
}

void testAllFlagCombinations()
{
  DefaultReceiveProtocol protocol;
  for (uint16_t flags = 0; flags < 256; ++flags) {
    auto bytes = encode(samplePacket());
    bytes[1] = static_cast<uint8_t>(flags);
    crc16::Append_CRC16_Check_Sum(bytes.data(), static_cast<uint32_t>(bytes.size()));
    ReceivedData data;
    check(protocol.decode(bytes, data) == DecodeStatus::Ok, "flag combination rejected");
    check(data.control.detect_color == (flags & 1U), "color flag mismatch");
    check(data.status.task_mode == ((flags >> 1) & 3U), "task mode flag mismatch");
    check(data.control.reset_tracker == ((flags & 8U) != 0), "reset flag mismatch");
    check(data.status.is_play == ((flags & 16U) != 0), "recording flag mismatch");
    check(data.control.change_target == ((flags & 32U) != 0), "target flag mismatch");
  }
}

void testPacketCopySizeGuard()
{
  bool short_rejected = false;
  bool long_rejected = false;
  try {
    fromVector(std::vector<uint8_t>(sizeof(ReceivePacket) - 1));
  } catch (const std::invalid_argument &) {
    short_rejected = true;
  }
  try {
    fromVector(std::vector<uint8_t>(sizeof(ReceivePacket) + 1));
  } catch (const std::invalid_argument &) {
    long_rejected = true;
  }
  check(short_rejected && long_rejected, "unsafe packet copy accepted");
}
}  // namespace

int main()
{
  const std::vector<std::pair<const char *, std::function<void()>>> tests = {
    {"valid packet fields", testValidDecode},
    {"independent wire layout", testIndependentWireFixture},
    {"empty / short / long input", testInvalidSizes},
    {"invalid header", testInvalidHeader},
    {"corrupt payload", testCorruptPayload},
    {"corrupt checksum", testCorruptChecksum},
    {"failed decode leaves output unchanged", testRejectedOutputUnchanged},
    {"three handlers and control order", testNormalDispatch},
    {"rejected frames have no side effects", testRejectedFramesNotDispatched},
    {"false flags and repeated frames", testFalseFlagsAndRepeatedFrames},
    {"task mode names and fallback", testTaskModeNames},
    {"handler order and extension", testRegistrationOrderAndExtension},
    {"replace protocol without changing handlers", testReplaceProtocol},
    {"invalid configuration", testInvalidConfiguration},
    {"all 256 flag combinations", testAllFlagCombinations},
    {"packet copy size guard", testPacketCopySizeGuard}};
  std::size_t passed = 0;
  for (const auto & test : tests) {
    try {
      test.second();
      ++passed;
      std::cout << "[PASS] " << test.first << '\n';
    } catch (const std::exception & ex) {
      std::cerr << "[FAIL] " << test.first << ": " << ex.what() << '\n';
    }
  }
  std::cout << passed << '/' << tests.size() << " tests passed\n";
  return passed == tests.size() ? 0 : 1;
}
