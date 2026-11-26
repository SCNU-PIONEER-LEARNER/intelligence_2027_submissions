// Copyright (c) 2022 ChenJun
// Licensed under the Apache-2.0 License.
// CRC16 校验接口：用于串口报文的完整性校验。
// Verify 检查帧尾校验和，Append 计算并写入帧尾校验和。

#ifndef RM_SERIAL_DRIVER__CRC_HPP_
#define RM_SERIAL_DRIVER__CRC_HPP_

#include <cstdint>

namespace crc16
{

uint32_t Verify_CRC16_Check_Sum(const uint8_t * pchMessage, uint32_t dwLength);

void Append_CRC16_Check_Sum(uint8_t * pchMessage, uint32_t dwLength);

}

#endif
