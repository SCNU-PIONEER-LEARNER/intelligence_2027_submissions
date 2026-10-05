#pragma once

#include <string>

class Building
{
public:
    Building(const std::string &type, int health);

    bool Survive() const;
    void TakeDamage(int damage);

    // main.cpp 会直接读取或修改这些状态，因此保留为 public。
    int Health_;
    bool Invincible;
    bool Protected;

private:
    std::string Type_;
};

class Robot
{
public:
    Robot(const std::string &type, int health, int attack, double hitRate);

    bool Survive() const;
    void Hit(Robot &target) const;
    void Hit(Building &target) const;

    // main.cpp 需要输出机器人的当前生命值。
    int Health_;

private:
    bool AttackHits() const;

    std::string Type_;
    int Attack_;
    double HitRate_;
};
