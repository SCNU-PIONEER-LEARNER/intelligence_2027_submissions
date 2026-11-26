// 接收消费者接口。
// B1-B7 每个业务块实现一个 consume()，由 receiveData() 统一分发。

#ifndef RM_SERIAL_DRIVER_RECEIVE_CONSUMER_HPP
#define RM_SERIAL_DRIVER_RECEIVE_CONSUMER_HPP

namespace rm_serial_driver{
    struct ReceivePacket;

    class ReceiveConsumer{
        public:
        virtual ~ReceiveConsumer() = default;
        virtual void consume(const ReceivePacket& packet) = 0;

    };
}
#endif
