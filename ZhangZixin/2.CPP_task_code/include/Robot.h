#ifndef ROBOT_H
#define ROBOT_H

#include <string>

class Building;  // 前向声明：Robot::Hit(Building&) 需要使用 Building 类型

// ============ 机器人 ============
class Robot
{
public:
    // 构造函数：类型 / 生命值 / 攻击力 / 命中率
    Robot(const std::string &type, int health, int attack, double hit_rate);

    void Hit(Building &target);  // 攻击建筑物
    void Hit(Robot &target);     // 攻击机器人

    bool Survive() const;        // 是否存活（生命值 > 0）

    // main.cpp 直接访问的公有成员
    int Health_ = 0;
    std::string Type_;

private:
    int attack_ = 0;      // 攻击力
    double hit_rate_ = 0.0;  // 命中率（0~1）
};

// ============ 建筑物（前哨站 / 基地） ============
class Building
{
public:
    Building(const std::string &type, int health);

    bool Survive() const;   // 是否存活（生命值 > 0）

    // main.cpp 直接访问的公有成员
    int Health_ = 0;
    bool Invincible = false;  // 无敌：前哨站存活时基地无敌
    bool Protected = false;   // 护盾：哨兵存活时基地护甲关闭（减伤）
    std::string Type_;
};

#endif  // ROBOT_H
