//头文件保护
#ifndef ROBOT_H
#define ROBOT_H
#include <string>
#include <vector>
#include <random>
using namespace std;



class Building{
    public:
    string type;
    int hp;
    
    bool Invincible=false;
    bool Protected=false;

    Building(string TYPE,int HP);
    bool Survive();
};

class Robot{

    public:
    string type;
    int hp;
    int atk;
    double rate;

    //构造函数声明
    Robot(string TYPE, int HP , int ATK , double RATE);

    void Hit(Robot& target);
    void Hit(Building& target);
    bool Survive();
};
template<typename T>
bool alive(std::vector<T>& vec){
    bool a=false;
    for(auto i:vec){
        if (i.Survive()){
            return true; 
        }
    }return false;
}

#endif