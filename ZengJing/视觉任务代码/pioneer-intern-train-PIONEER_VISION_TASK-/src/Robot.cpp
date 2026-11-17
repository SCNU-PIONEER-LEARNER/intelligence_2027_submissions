#include "../include/Robot.h"
#include <cstdlib>
Robot::Robot(std::string type, int hp, int attack, double hit_rate)
: m_type(type), m_hp(hp), m_attack(attack), m_hit_rate(hit_rate)
{
    Health_=m_hp;
}
Robot::Robot(const Robot& r):m_type(r.m_type), m_hp(r.m_hp), m_attack(r.m_attack), m_hit_rate(r.m_hit_rate)
{
    Health_=r.Health_;
}       
bool Robot::Survive()
{
    return Health_>0;
}
void Robot::Hit(Building &b)
{
    double rate = static_cast<double>(rand()) / RAND_MAX;        // 得到 [0.0 , 1.0] 的随机小数，包含0，包含1
    //用来判断是否能够击中
    if(m_hit_rate>rate)
    {
        b.Health_-=m_attack;
        if (b.Health_ < 0)
        {
            b.Health_ = 0;    //生命值最低为0
        }
    }else{
        return;
    }
}
void Robot::Hit(Robot &r)
{
    double rate = static_cast<double>(rand()) / RAND_MAX;        // 得到 [0.0 , 1.0] 的随机小数，包含0，包含1
    //用来判断是否能够击中
    if(m_hit_rate>rate)
    {
        r.Health_-=m_attack;
        if (r.Health_ < 0)
        {
            r.Health_ = 0;    //生命值最低为0
        }
    }else{
        return;
    }
}
    Building::Building(std::string type, int hp) :m_type(type), m_hp(hp)
    {
        Health_=m_hp;
    }
    Building::Building(const Building& r): m_type(r.m_type), m_hp(r.m_hp), Invincible(r.Invincible), Protected(r.Protected)
    {
        Health_=r.Health_;
    }          
    bool Building::Survive()
    {
        return Health_>0;
    }
