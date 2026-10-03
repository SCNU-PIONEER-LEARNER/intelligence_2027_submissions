#include "Robot.hpp"
#include <cstdlib>  


Robot::Robot(std::string type, double health, int attack, double hitrate)
    : Type_(type), Health_(health), Attack_(attack), HitRate_(hitrate)
{
}

bool Robot::Survive() const
{
    return Health_ > 0;
}

void Robot::Hit(Building& target)
{
    if(target.Invincible)
    {
        return;
    }
    if(target.Protected)
    {
        return;
    }
    double r = static_cast<double>(rand()) / RAND_MAX;
    if(r < HitRate_)
    {
        target.Health_ -= Attack_;
        if(target.Health_ < 0)
        {
            target.Health_ = 0;
        }
    }
}

void Robot::Hit(Robot& target)
{
    double r = static_cast<double>(rand()) / RAND_MAX;
    if(r < HitRate_)
    {
        target.Health_ -= Attack_;
        if(target.Health_ < 0)
        {
            target.Health_ = 0;
        }
    }
}


Building::Building(std::string type, double health)
    : Type_(type), Health_(health), Invincible(false), Protected(false)
{
}

bool Building::Survive() const
{
    return Health_ > 0;
}
