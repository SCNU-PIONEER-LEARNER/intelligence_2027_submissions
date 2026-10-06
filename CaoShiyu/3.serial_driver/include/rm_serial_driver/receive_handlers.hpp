// SPDX-License-Identifier: Apache-2.0
#ifndef RM_SERIAL_DRIVER__RECEIVE_HANDLERS_HPP_
#define RM_SERIAL_DRIVER__RECEIVE_HANDLERS_HPP_

#include "rm_serial_driver/receive_data.hpp"

namespace rm_serial_driver
{
// Narrow output interfaces: each handler uses only its own capabilities.
class IControlActions
{
public:
  virtual ~IControlActions() = default;
  virtual void syncDetectColor(uint8_t color) = 0;
  virtual void resetTracker() = 0;
  virtual void changeTarget() = 0;
};

class IStatusOutput
{
public:
  virtual ~IStatusOutput() = default;
  virtual void publishStatus(const StatusData & status) = 0;
};

class IGimbalOutput
{
public:
  virtual ~IGimbalOutput() = default;
  virtual void publishGimbal(const GimbalData & gimbal) = 0;
};

class IReceiveHandler
{
public:
  virtual ~IReceiveHandler() = default;
  virtual void handle(const ReceivedData & data) = 0;
};

class ControlHandler final : public IReceiveHandler
{
public:
  explicit ControlHandler(IControlActions & actions) : actions_(actions) {}

  void handle(const ReceivedData & data) override
  {
    actions_.syncDetectColor(data.control.detect_color);
    if (data.control.reset_tracker) {
      actions_.resetTracker();
    }
    if (data.control.change_target) {
      actions_.changeTarget();
    }
  }

private:
  IControlActions & actions_;
};

class StatusHandler final : public IReceiveHandler
{
public:
  explicit StatusHandler(IStatusOutput & output) : output_(output) {}

  void handle(const ReceivedData & data) override
  {
    output_.publishStatus(data.status);
  }

private:
  IStatusOutput & output_;
};

class GimbalHandler final : public IReceiveHandler
{
public:
  explicit GimbalHandler(IGimbalOutput & output) : output_(output) {}

  void handle(const ReceivedData & data) override
  {
    output_.publishGimbal(data.gimbal);
  }

private:
  IGimbalOutput & output_;
};

inline const char * taskModeName(uint8_t mode)
{
  switch (mode) {
    case 1: return "small_buff";
    case 2: return "large_buff";
    default: return "aim";
  }
}
}  // namespace rm_serial_driver

#endif  // RM_SERIAL_DRIVER__RECEIVE_HANDLERS_HPP_
