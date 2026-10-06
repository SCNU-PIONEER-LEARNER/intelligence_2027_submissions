// SPDX-License-Identifier: Apache-2.0
#ifndef RM_SERIAL_DRIVER__RECEIVE_PIPELINE_HPP_
#define RM_SERIAL_DRIVER__RECEIVE_PIPELINE_HPP_

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "rm_serial_driver/receive_handlers.hpp"
#include "rm_serial_driver/receive_protocol.hpp"

namespace rm_serial_driver
{
// Own the protocol and handlers; register everything before starting reception.
// Output adapters referenced by handlers must outlive this pipeline.
class ReceivePipeline
{
public:
  explicit ReceivePipeline(std::unique_ptr<IReceiveProtocol> protocol)
  : protocol_(std::move(protocol))
  {
    if (!protocol_ || protocol_->frameSize() == 0) {
      throw std::invalid_argument("ReceivePipeline requires a nonempty-frame protocol");
    }
  }

  uint8_t headerByte() const { return protocol_->headerByte(); }
  std::size_t frameSize() const { return protocol_->frameSize(); }

  void addHandler(std::unique_ptr<IReceiveHandler> handler)
  {
    if (!handler) {
      throw std::invalid_argument("Cannot register a null receive handler");
    }
    handlers_.push_back(std::move(handler));
  }

  DecodeStatus processFrame(const std::vector<uint8_t> & bytes)
  {
    ReceivedData data;
    const DecodeStatus result = protocol_->decode(bytes, data);
    if (result != DecodeStatus::Ok) {
      return result;
    }
    for (const auto & handler : handlers_) {
      handler->handle(data);
    }
    return DecodeStatus::Ok;
  }

private:
  std::unique_ptr<IReceiveProtocol> protocol_;
  std::vector<std::unique_ptr<IReceiveHandler>> handlers_;
};
}  // namespace rm_serial_driver

#endif  // RM_SERIAL_DRIVER__RECEIVE_PIPELINE_HPP_
