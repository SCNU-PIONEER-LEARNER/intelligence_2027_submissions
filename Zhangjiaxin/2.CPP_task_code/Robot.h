#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <cstdlib>
#include <type_traits>

// 前向声明
class Building;

// ==================== Robot 类 ====================
class Robot
{
public:
    Robot() : Name_("Unknown"), Health_(0), Damage_(0), Hit_rate_(0.0) {}
    Robot(std::string name, int health, int damage, double hit_rate)
        : Name_(name), Health_(health), Damage_(damage), Hit_rate_(hit_rate) {
    }

    // 攻击函数（模板，支持攻击 Robot 或 Building）
    template <typename T>
    void Hit(T& target)
    {
        if (!target.Survive())
            return;

        // 命中判定
        double r = static_cast<double>(rand()) / RAND_MAX;
        if (r > Hit_rate_)
            return;

        // 如果目标是建筑物，需要检查无敌和护盾
        if constexpr (std::is_same_v<T, Building>)
        {
            if (target.Invincible || target.Protected)
                return;
        }

        target.Health_ -= Damage_;
        if (target.Health_ < 0)
            target.Health_ = 0;
    }

    bool Survive() const { return Health_ > 0; }

public:
    std::string Name_;
    int Health_;
    int Damage_;
    double Hit_rate_;
};

// ==================== Building 类 ====================
class Building
{
public:
    Building() : Name_("Unknown"), Health_(0), Invincible(false), Protected(false) {}
    Building(std::string name, int health)
        : Name_(name), Health_(health), Invincible(false), Protected(false) {
    }

    bool Survive() const { return Health_ > 0; }

public:
    std::string Name_;
    int Health_;
    bool Invincible; // 无敌状态
    bool Protected;  // 护盾状态
};

#endif // ROBOT_H#pragma once
