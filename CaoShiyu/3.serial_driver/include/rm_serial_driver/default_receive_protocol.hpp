// SPDX-License-Identifier: Apache-2.0
#ifndef RM_SERIAL_DRIVER__DEFAULT_RECEIVE_PROTOCOL_HPP_
#define RM_SERIAL_DRIVER__DEFAULT_RECEIVE_PROTOCOL_HPP_

#include "rm_serial_driver/receive_protocol.hpp"

namespace rm_serial_driver
{
// Adapter for the ReceivePacket format supplied in the original task.
class DefaultReceiveProtocol final : public IReceiveProtocol
{
public:
  uint8_t headerByte() const override;
  std::size_t frameSize() const override;
  DecodeStatus decode(
    const std::vector<uint8_t> & bytes, ReceivedData & output) const override;
};
}  // namespace rm_serial_driver

#endif  // RM_SERIAL_DRIVER__DEFAULT_RECEIVE_PROTOCOL_HPP_
