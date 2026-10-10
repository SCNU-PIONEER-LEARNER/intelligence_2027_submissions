#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

class Building;

class Robot
{
public:
    string name;
    double Health_;   
    int attack;        
    double hit_rate;   

    Robot(string n, double hp, int atk, double rate);


    void Hit(Robot &target);
    void Hit(Building &target);
};


class Building
{
public:
    string name;
    double Health_;
    bool Invincible; 

    Building(string n, double hp);

    bool Survive();

};

#endif