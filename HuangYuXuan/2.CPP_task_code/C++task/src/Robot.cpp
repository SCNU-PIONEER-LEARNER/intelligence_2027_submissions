// 写函数定义
#include "../include/Robot.h"
#include <string>
// 类外定义函数
Building::Building(string name, int HP) : B_name(name), Health_(HP) {}
Robot::Robot(string name, int HP, int Attack, double goal)
    : R_name(name), Health_(HP), AttackPower(Attack), goal_rate(goal) {}
// 定义Hit函数
void Robot::Hit(Robot &target) {
  target.Health_ -= this->AttackPower;
  // 如果血量低于0 ，让他保持为0即可
  if (target.Health_ < 0) {
    target.Health_ = 0;
  }
  cout << R_name << "攻击了" << target.R_name << ",造成了" << AttackPower
       << "伤害" << endl;
}
void Robot::Hit(Building &target) {
  target.Health_ -= this->AttackPower;
  // 如果血量低于0 ，让他保持为0即可
  if (target.Health_ < 0) {
    target.Health_ = 0;
  }
  cout << R_name << "攻击了" << target.B_name << ",造成了" << AttackPower
       << "伤害" << endl;
}
