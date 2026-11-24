#ifndef RMQ_PACKET_CODEC_HPP
#define RMQ_PACKET_CODEC_HPP 
#include <cstdint>
#include <cstddef>
#include <vector>
#include "auto_aim_interfaces/msg/gimbal_cmd.hpp"
#include "rm_serial_driver/packet.hpp"
namespace rm_serial_driver
{
    /// 按照19期视频把数据处理部分写死的抽象成一个结构体,方便后续修改，就不存在写死的数据了
    //协议改了就改一下结构体，0，1，2分别对应啥，代码里面就写task_mode==aim,task_mode==smallbuff,task_mode==large_buff这种，不要把逻辑写死成0，1，2
    typedef ReceivePacket ControllerCommand; //那我直接结构体赋值就完了，数据包的问题就交给前期的校验和判断还有处理完的判断
      ////////////////////diff地狱是说要改很多处地方，改漏一处直接完蛋
      /////////////////////所以根本目的就是只需要改一处地方就行了，这样就不会存在漏改，然后各个函数之间各司其职，这样或许就是解偶
    enum class TaskMode : uint8_t
{
  aim = 0,
  small_buff = 1,
  large_buff = 2
};
inline TaskMode toTaskMode(uint8_t raw)
{
  return static_cast<TaskMode>(raw);
}
class PacketCodec
{
public:
     void copydata(const uint8_t * data,size_t length);//代码里面写死判断0x5A，还有那种数据处理方式只能处理这种，我直接先全部复制进来
     bool decode(ControllerCommand & out);
     std::vector<uint8_t> encode(const auto_aim_interfaces::msg::GimbalCmd & cmd,bool is_buff);
    std::vector<uint8_t> real_data;
};
}
#endif
