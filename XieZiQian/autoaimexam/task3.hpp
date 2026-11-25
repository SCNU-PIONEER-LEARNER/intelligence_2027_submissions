/**
本包实现了当新增命令码与数据时，只需加代码而不需改代码
*/
#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>
#include <utility> // 用于 std::move

namespace RefereeSystem {

// 使用别名
using CommandID = uint16_t;           // 命令码类型（2字节）
using Payload = std::vector<uint8_t>; // 数据段类型（不定长字节流）

/**
 * @brief 命令处理接口（抽象基类）
 * 所有的具体处理逻辑都必须继承此类并重写 handle 函数。
 */
class ICommandHandler { 
public:
    virtual ~ICommandHandler() = default;
    virtual void handle(const Payload& data) = 0;
};

/**
 * @brief 命令分发器
 * 负责维护 命令码 与 处理函数 的映射关系，并负责分发数据。
 */
class CommandDispatcher {
public:
    
    CommandDispatcher() = default;
    ~CommandDispatcher() = default;

    // 禁用拷贝构造和赋值，防止由于拷贝导致的指针混乱
    CommandDispatcher(const CommandDispatcher&) = delete;
    CommandDispatcher& operator=(const CommandDispatcher&) = delete;

    /**
     * @brief 注册命令处理器
     * @param cmdId 命令码
     * @param handler 处理器的智能指针
     */
    void registerHandler(CommandID cmdId, std::shared_ptr<ICommandHandler> handler) {
        if (handler != nullptr) {
            handlers_[cmdId] = std::move(handler); //避免引用计数增加开销
        }
    }

    /**
     * @brief 分发数据（接收到底层传来的数据后调用此函数）
     * @param cmdId 解析出来的命令码
     * @param data 解析出来的数据段
     */
    void dispatch(CommandID cmdId, const Payload& data) {
        auto it = handlers_.find(cmdId);
        if (it != handlers_.end()) {
            // 找到了对应的处理器，调用多态方法处理数据
            it->second->handle(data);
        } else {
            // 找不到的命令码（可能是未注册或者未知的）
            // 在真实比赛中，这里通常会记录日志，或者直接忽略
            std::cerr << "[Warning] 收到未知的命令码: 0x" 
                      << std::hex << cmdId << std::dec 
                      << "，数据长度: " << data.size() << std::endl;
        }
    }

private:
    // STL哈希表，将命令码映射到处理器对象上
    std::unordered_map<CommandID, std::shared_ptr<ICommandHandler>> handlers_;
};

}