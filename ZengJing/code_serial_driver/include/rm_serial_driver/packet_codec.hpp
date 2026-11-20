#ifndef RMQ_PACKET_CODEC_HPP
#define RMQ_PACKET_CODEC_HPP 
#include <cstdint>
#include <cstddef>
#include <vector>
#include "auto_aim_interfaces/msg/gimbal_cmd.hpp"
namespace rm_serial_driver
{
    /// 按照19期视频把数据处理部分写死的抽象成一个结构体,方便后续修改，就不存在写死的数据了
    //协议改了就改一下结构体，0，1，2分别对应啥，代码里面就写task_mode==aim,task_mode==smallbuff,task_mode==large_buff这种，不要把逻辑写死成0，1，2
    struct ControllerCommand
    {
        uint8_t detect_color=0;
        enum class Task
        {
            aim=0,
            small_buff=1,
            large_buff=2
        };
        Task task_mode=Task::aim;
        bool reset_tracker=false;
        bool change_target=false;
        bool is_play=true;
        double roll=0.0;
        double pitch=0.0;
        double yaw=0.0;
        double bullet_speed=0.0;
    } ;

class PacketCodec
{
public:
    void copydata(const uint8_t * data,size_t length);//代码里面写死判断0x5A，还有那种数据处理方式只能处理这种，我直接先全部复制进来
    bool decode(ControllerCommand & out);
    std::vector<uint8_t> encode(auto_aim_interfaces::msg::GimbalCmd & cmd,bool is_buff);
    std::vector<uint8_t> real_data;
};
}
#endif
