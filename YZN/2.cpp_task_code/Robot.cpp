#include "../include/Robot.h"

Robot::Robot(string n, double hp, int atk, double rate)
{
    name = n;
    Health_ = hp;
    attack = atk;
    hit_rate = rate;
}

void Robot::Hit(Robot &target)
{
    int rand_val = rand() % 100; 
    if(rand_val < hit_rate*100) 
    {
        target.Health_ -= attack;
        if(target.Health_ < 0) target.Health_ = 0;
    }
}

void Robot::Hit(Building &target)
{
    if(target.Invincible == true)
    {
        return;
    }

    int rand_val = rand() % 100; 
    if(rand_val < hit_rate*100)
    {
        target.Health_ -= attack;
        if(target.Health_ < 0) target.Health_ = 0;
    }
}


Building::Building(string n, double hp)
{
    name = n;
    Health_ = hp;
    Invincible = false;
}

bool Building::Survive()
{
    if(Health_ > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
