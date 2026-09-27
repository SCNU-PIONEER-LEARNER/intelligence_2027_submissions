#include"robot.h"
#include<cstdlib>

Robot::Robot(std::string type,int health,int attack,double hitRate)
	:Type_(type),Health_(health),Attack_(attack),HitRate_(hitRate),
	Invincible(false),Protected(false){ }

Building::Building(std::string type,int health)
	:Type_(type),Health_(health),Invincible(false),Protected(false){ }

bool Building::Survive()const {
	return Health_ > 0;
}

bool Robot::Survive()const {
	return Health_ > 0;
}

void Robot::Hit(Building& target) {
	if (target.Invincible) { return; }

	int actual_damage = Attack_;
	if (target.Protected) {
		actual_damage = actual_damage / 2;
	}

	double random_val = static_cast<double>(rand()) / RAND_MAX;
	if (random_val <= HitRate_) {
		target.Health_ -= actual_damage;
		if (target.Health_ < 0)target.Health_ = 0;
	}

}

void Robot::Hit(Robot& target) {


	int actual_damage = Attack_;
	if (target.Protected) {
		actual_damage = actual_damage / 2;
	}

	double random_val = static_cast<double>(rand()) / RAND_MAX;
	if (random_val <= HitRate_) {
		target.Health_ -= actual_damage;
		if (target.Health_ < 0)target.Health_ = 0;
	}
}