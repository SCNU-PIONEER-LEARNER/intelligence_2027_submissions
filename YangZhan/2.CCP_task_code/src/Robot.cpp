#include "Robot.h"
#include <string>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

std::random_device rd;
std::mt19937 gen(rd());//一定要放外面

Robot::Robot(string TYPE, int HP , int ATK , double RATE):
    type(TYPE), hp(HP), rate(RATE){}
Building::Building(string TYPE,int HP):type(TYPE),hp(HP){}


void Robot:: Hit(Robot& target){
    bernoulli_distribution dist(rate);
    if(dist(gen)){target.hp-=atk;}
    if(!target.Survive()){target.hp=0;}
}
void Robot:: Hit(Building& target){
    bernoulli_distribution dist(rate);
    if(dist(gen) && !target.Invincible){
        if(!target.Protected){target.hp-=atk;}
        else{target.hp-=atk/2;}
    if(!target.Survive()){target.hp=0;}
}
}
bool Building:: Survive(){return hp>0;}

bool Robot::Survive(){return hp>0;}




