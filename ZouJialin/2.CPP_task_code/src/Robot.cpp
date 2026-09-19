#include "../include/Robot.h"
#include <cstdlib>
Building::Building(std::string type, int health)
{
    type_ = type;
    Health_ = health;
    Invincible = false;
    Protected = false;
}

bool Building::Survive() const
{
    return Health_ > 0;
}

Robot::Robot(std::string type, int health, int attack, double hit_rate)
{
    type_ = type;
    Health_ = health;
    attack_ = attack;
    hit_rate_ = hit_rate;
}

bool Robot::Survive() const
{
    return Health_ > 0;
}

void Robot::Hit(Building& target)
{
    // 命中率判定：rand()/RAND_MAX 得到一个 0~1 的数
    double chance = static_cast<double>(rand()) / RAND_MAX;
    if (chance > hit_rate_)
        return;

    if (target.Invincible)
        return;

    int damage = attack_;
    if (target.Protected)
        damage = attack_ / 2;

    target.Health_ -= damage;
}

void Robot::Hit(Robot& target)
{
    double chance = static_cast<double>(rand()) / RAND_MAX;
    if (chance > hit_rate_)
        return;

    target.Health_ -= attack_;
}