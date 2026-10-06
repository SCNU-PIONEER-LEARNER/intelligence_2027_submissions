// SPDX-License-Identifier: Apache-2.0
#ifndef RM_SERIAL_DRIVER__RECEIVE_PROTOCOL_HPP_
#define RM_SERIAL_DRIVER__RECEIVE_PROTOCOL_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>

#include "rm_serial_driver/receive_data.hpp"

namespace rm_serial_driver
{
enum class DecodeStatus
{
  Ok,
  InvalidSize,
  InvalidHeader,
  InvalidChecksum
};

// Fixed-length, one-byte-header protocol contract.
// A different wire layout is implemented behind this interface.
class IReceiveProtocol
{
public:
  virtual ~IReceiveProtocol() = default;
  virtual uint8_t headerByte() const = 0;
  virtual std::size_t frameSize() const = 0;
  // A rejected frame must leave output unchanged.
  virtual DecodeStatus decode(
    const std::vector<uint8_t> & bytes, ReceivedData & output) const = 0;
};
}  // namespace rm_serial_driver

#endif  // RM_SERIAL_DRIVER__RECEIVE_PROTOCOL_HPP_
