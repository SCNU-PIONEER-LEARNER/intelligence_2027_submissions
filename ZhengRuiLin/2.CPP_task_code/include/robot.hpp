#ifndef ROBOT_H
#define ROBOT_H

#include <string>

// 机器人类
class Robot
{
public:
    std::string Type_;
    double Health_;
    int Attack_;
    double HitRate_;

    // 构造函数
    Robot(std::string type, double health, int attack, double hitrate);

    // 判断是否存活
    bool Survive() const;

    // 攻击重载：攻击建筑物
    void Hit(class Building& target);
    // 攻击重载：攻击机器人
    void Hit(Robot& target);
};
class Building
{
public:
    std::string Type_;
    double Health_;
    bool Invincible;  
    bool Protected;   
    Building(std::string type, double health);
    bool Survive() const;
};

#endif
