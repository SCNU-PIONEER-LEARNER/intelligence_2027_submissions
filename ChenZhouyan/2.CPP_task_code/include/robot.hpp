#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <string>

// Building 只在 Hit 的参数里用到（引用），先声明一下就够
class Building;

// 机器人类：红蓝双方共用的兵种，既能打机器人也能打建筑
class Robot
{
public:
    std::string Name; // 兵种名：Guard / Infantry / Hero
    int Health_;      // 剩余生命值
    int Attack_;      // 攻击力
    double HitRate_;  // 命中率，0~1

    Robot(std::string name = "", int health = 0, int attack = 0, double hit_rate = 0.0);

    bool Survive() const;        // 还活着吗
    void Hit(Robot &target);     // 攻击对方机器人
    void Hit(Building &target);  // 攻击对方建筑
};

// 建筑类：前哨站和基地，只挨打不还手
class Building
{
public:
    std::string Name;
    int Health_;
    bool Invincible; // 无敌：前哨站没被摧毁时，基地无敌
    bool Protected;  // 护盾：哨兵还活着时，基地受护盾保护

    Building(std::string name = "", int health = 0);

    bool Survive() const;
};

#endif
