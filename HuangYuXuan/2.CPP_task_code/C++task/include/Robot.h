// 补充头文件
// 头文件保护
#ifndef Robot_h
#define Robot_h
#include <iostream>
using namespace std;
class Building {
public:
  string B_name;
  bool Protected = false;
  bool Invincible = false;
  int Health_;
  Building(string name, int HP); // 先声明
  bool Survive() { return Health_ > 0; }
};
class Robot {
public:
  friend class Building;
  string R_name;
  int Health_;
  int AttackPower;
  double goal_rate;
  Robot(string name, int HP, int Attack, double goal); // 先声明
  void Hit(Building &target);
  void Hit(Robot &target);
  bool Survive() { return Health_ > 0; }
};
#endif