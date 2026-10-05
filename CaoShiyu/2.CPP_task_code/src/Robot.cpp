#include "../include/Robot.h"

#include <algorithm>
#include <cstdlib>

Building::Building(const std::string &type, int health)
    : Health_(health), Invincible(false), Protected(false), Type_(type)
{
}

bool Building::Survive() const
{
    return Health_ > 0;
}

void Building::TakeDamage(int damage)
{
    // 建筑物被摧毁或处于无敌状态时不再扣血。
    if (!Survive() || Invincible)
    {
        return;
    }

    // 有护盾时只承受一半伤害。
    int actualDamage = Protected ? damage / 2 : damage;
    Health_ = std::max(0, Health_ - actualDamage);
}

Robot::Robot(const std::string &type, int health, int attack, double hitRate)
    : Health_(health), Type_(type), Attack_(attack), HitRate_(hitRate)
{
}

bool Robot::Survive() const
{
    return Health_ > 0;
}

bool Robot::AttackHits() const
{
    double randomValue = static_cast<double>(std::rand()) / RAND_MAX;
    return randomValue < HitRate_;
}

void Robot::Hit(Robot &target) const
{
    if (!Survive() || !target.Survive() || !AttackHits())
    {
        return;
    }

    target.Health_ = std::max(0, target.Health_ - Attack_);
}

void Robot::Hit(Building &target) const
{
    if (!Survive() || !target.Survive() || !AttackHits())
    {
        return;
    }

    target.TakeDamage(Attack_);
}
