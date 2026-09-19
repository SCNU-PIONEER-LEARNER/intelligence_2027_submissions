#ifndef ROBOT_H
#define ROBOT_H

#include<string>

class Building
{
    public:
    Building(std::string type,int health);

    bool Survive() const;

    int Health_;
    bool Invincible;
    bool Protected;
    std::string type_;
};
class Robot
{
    public:
    Robot(std::string type, int health, int attack, double hit_rate);
    void Hit(Building& target);   // 攻击建筑物
    void Hit(Robot& target);      // 攻击机器人（函数重载）
    bool Survive() const;
    int Health_;

private:
    std::string type_;
    int attack_;
    double hit_rate_;
};
#endif