#ifndef ROBOT_H
#define ROBOT_H
#include <string>

// 先声明Building类，因为Robot的Hit函数要接收Building对象
class Building;

class Robot
{
public:
    std::string name;
    double Health_;      // 生命值
    double attack;       // 攻击力
    double hitRate;      // 命中率

    // 构造函数
    Robot(std::string n, double hp, double atk, double rate);

    // 判断是否存活：生命值>0返回true
    bool Survive() const;

    // 攻击机器人
    void Hit(Robot &target);

    // 攻击建筑物（重载Hit函数）
    void Hit(Building &target);
};

class Building
{
public:
    std::string name;
    double Health_;
    bool Invincible;   // 无敌标记
    bool Protected;    // 护盾标记

    // 建筑物构造函数
    Building(std::string n, double hp);

    // 判断建筑是否存活
    bool Survive() const;
};

#endif
