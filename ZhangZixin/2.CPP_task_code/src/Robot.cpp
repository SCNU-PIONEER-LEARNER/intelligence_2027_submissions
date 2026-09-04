#include "Robot.h"

#include <cstdlib>  // rand()

// ============ Robot 实现 ============
Robot::Robot(const std::string &type, int health, int attack, double hit_rate)
    : Type_(type), Health_(health), attack_(attack), hit_rate_(hit_rate)
{
}

// 攻击建筑物
void Robot::Hit(Building &target)
{
    // 按命中率判定是否命中
    if ((rand() % 100) / 100.0 >= hit_rate_)
        return;  // 未命中，本次攻击无效

    // 基地无敌机制：前哨站未被摧毁前，基地不受任何伤害
    if (target.Invincible)
        return;

    int damage = attack_;
    // 基地护盾机制：哨兵存活时基地护甲关闭，伤害减半（可自行调整）
    if (target.Protected)
        damage /= 2;

    target.Health_ -= damage;
    if (target.Health_ < 0)
        target.Health_ = 0;  // 生命值不低于 0
}

// 攻击机器人
void Robot::Hit(Robot &target)
{
    // 按命中率判定是否命中
    if ((rand() % 100) / 100.0 >= hit_rate_)
        return;

    target.Health_ -= attack_;
    if (target.Health_ < 0)
        target.Health_ = 0;
}

bool Robot::Survive() const
{
    return Health_ > 0;
}

// ============ Building 实现 ============
Building::Building(const std::string &type, int health)
    : Type_(type), Health_(health)
{
}

bool Building::Survive() const
{
    return Health_ > 0;
}
