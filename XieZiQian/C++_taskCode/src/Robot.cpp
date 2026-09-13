#include "../include/Robot.h"
#include <iostream>
#include <cstdlib>
using namespace std;

//class Robot
Robot::Robot(string type,int hp,int atk,double rate):Type_(type),Health_(hp),Atk_(atk),Rate_(rate){}
bool Robot::Survive(){
    return Health_ > 0;
}
void Robot::Hit(Building& building){
    if (!Survive() || !building.Survive()){
        return;
    }
    if(building.Invincible){
        cout << building.Type_ << " 处于无敌状态，攻击无效！" << endl;
        return;
    }
    if (building.Protected)
    {
        cout << building.Type_ << " 护盾未破，攻击无效！" << endl;
        return;
    }
    if (static_cast<double>(rand()) / RAND_MAX > Rate_)
    {
        cout << Type_ << " 未命中 " << building.Type_ << endl;
        return;
    }
    building.Health_ -= Atk_;
    if(building.Health_ < 0){
        building.Health_ = 0;
    }
    cout << Type_ << " 命中 " << building.Type_
              << "，造成 " << Atk_ << " 点伤害，剩余血量 "
              << building.Health_ << endl;

    if (!building.Survive()){
        cout << building.Type_ << " 阵亡！" << endl;
    }
}
void Robot::Hit(Robot& robot){
    if (!Survive() || !robot.Survive()){
        return;
    }
        
    if (static_cast<double>(rand()) / RAND_MAX > Rate_)
    {
        cout << Type_ << " 未命中 " << robot.Type_ << endl;
        return;
    }

    robot.Health_ -= Atk_;
    if(robot.Health_ < 0){
        robot.Health_ = 0;
    }
    cout << Type_ << " 命中 " << robot.Type_
              << "，造成 " << Atk_ << " 点伤害，剩余血量 "
              << robot.Health_ << endl;

    if (!robot.Survive()){
        cout << robot.Type_ << " 阵亡！" << endl;
    }

}


//class Building
Building::Building(string type,int hp):Type_(type),Health_(hp){}
bool Building::Survive(){
    return Health_ > 0;
}

