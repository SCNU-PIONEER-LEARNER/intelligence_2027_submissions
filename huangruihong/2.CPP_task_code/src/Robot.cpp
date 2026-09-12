#include "include/Robot.hpp"
#include <cstdlib>
#include <ctime>

// Robot构造函数
Robot::Robot(std::string n, double hp, double atk, double rate)
    : name(n), Health_(hp), attack(atk), hitRate(rate)
{
    
}

bool Robot::Survive() const
{
    return Health_ > 0;
}

// 攻击机器人
void Robot::Hit(Robot &target)
{
    // 已经死亡不能攻击
    if (!this->Survive()) return;
    // 目标已经死亡，不攻击
    if (!target.Survive()) return;

    // 随机判断是否命中
    double randVal = static_cast<double>(rand()) / RAND_MAX;
    if(randVal < hitRate)
    {
        target.Health_ -= attack;
        if(target.Health_ < 0) target.Health_ = 0;
    }
}

// 攻击建筑物
void Robot::Hit(Building &target)
{
    // 攻击者死亡，不能攻击
    if (!this->Survive()) return;
    // 目标建筑已经摧毁
    if (!target.Survive()) return;
    // 如果建筑无敌，直接跳过攻击
    if(target.Invincible) return;
    // 如果建筑处于护盾保护，伤害减半
    double realDmg = attack;
    if(target.Protected)
    {
        realDmg = attack * 0.5;
    }
    target.Health_ -= realDmg;
    if(target.Health_ < 0) target.Health_ = 0;
}

// Building类实现
Building::Building(std::string n, double hp)
    : name(n), Health_(hp), Invincible(false), Protected(false)
{}

bool Building::Survive() const
{
    return Health_ > 0;
}
