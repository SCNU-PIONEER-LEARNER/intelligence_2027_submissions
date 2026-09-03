#ifndef ROBOT_H
#define ROBOT_H
#include <string>
class Building;
class Robot
{
public:
   std::string m_type;    // 机器人类型
    int m_hp;             // 生命值
    int m_attack;         // 攻击力
    double m_hit_rate;    // 命中率
    double Health_;       //实际剩余生命值
    Robot(std::string type, int hp, int attack, double hit_rate);
    Robot(const Robot& r);   
    bool Survive();
    void Hit(Building &b);
    void Hit(Robot &r);
};
class Building
{
public:
   std::string m_type;    // 建筑物类型
    int m_hp;             // 生命值
    bool Invincible = false;
    bool Protected = false;
    double Health_;       //实际剩余生命值
    Building(std::string type, int hp);
    Building(const Building& r);        
    bool Survive();
};
#endif