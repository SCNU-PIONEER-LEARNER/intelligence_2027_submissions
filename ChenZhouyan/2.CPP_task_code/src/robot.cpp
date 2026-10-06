#include "../include/robot.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

// 构造：四个参数依次是 兵种名 生命值 攻击力 命中率
Robot::Robot(string name, int health, int attack, double hit_rate)
    : Name(name), Health_(health), Attack_(attack), HitRate_(hit_rate) {}

bool Robot::Survive() const
{
    return Health_ > 0;
}

// 攻击对方机器人：被摧毁的不能开火，也不鞭尸
void Robot::Hit(Robot &target)
{
    if (!Survive())
        return;
    if (!target.Survive())
        return;

    // 掷命中率，rand()%100 落在命中率区间内算命中
    if (rand() % 100 < HitRate_ * 100)
    {
        target.Health_ -= Attack_;
        cout << Name << " 命中 " << target.Name << "，造成 " << Attack_ << " 点伤害";
        if (!target.Survive())
            cout << "，" << target.Name << " 被摧毁！";
        cout << endl;
    }
    else
    {
        cout << Name << " 攻击 " << target.Name << " 失误" << endl;
    }
}

// 攻击对方建筑：无敌和护盾都会把攻击挡下来
void Robot::Hit(Building &target)
{
    if (!Survive())
        return;

    if (target.Invincible)
    {
        cout << target.Name << " 处于无敌状态，" << Name << " 的攻击无效" << endl;
        return;
    }
    if (target.Protected)
    {
        cout << target.Name << " 受护盾保护，" << Name << " 的攻击无效" << endl;
        return;
    }

    if (rand() % 100 < HitRate_ * 100)
    {
        target.Health_ -= Attack_;
        cout << Name << " 命中 " << target.Name << "，造成 " << Attack_ << " 点伤害";
        if (!target.Survive())
            cout << "，" << target.Name << " 被摧毁！";
        cout << endl;
    }
    else
    {
        cout << Name << " 攻击 " << target.Name << " 失误" << endl;
    }
}

// 建筑构造：初始都既不无敌也不受保护
Building::Building(string name, int health)
    : Name(name), Health_(health), Invincible(false), Protected(false) {}

bool Building::Survive() const
{
    return Health_ > 0;
}
