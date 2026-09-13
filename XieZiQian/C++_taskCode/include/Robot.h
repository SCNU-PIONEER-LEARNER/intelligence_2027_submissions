#ifndef ROBOT_H
#define ROBOT_H

#include<string>


class Building;
class Robot{
public:
    Robot(std::string type,int hp,int atk,double rate);
    bool Survive();
    void Hit(Building& building);
    void Hit(Robot& robot);
    std::string Type_;
    int Health_;
    int Atk_;
    double Rate_;
    
};

class Building{
public:
    Building(std::string type,int hp);
    bool Survive();
    std::string Type_;
    int Health_;
    bool Invincible;
    bool Protected;
    
};

#endif