#ifndef ROBOT_H
#define ROBOT_H

#include<iostream>
#include<string>

class Building ;

class Robot {
public:
    std::string Type_;
	int Health_;
	int Attack_;
	double HitRate_;
	bool Invincible;
	bool Protected;


	Robot(std::string type, int health, int attack, double hitRate);

	bool Survive()const;

	void Hit(Building& target);

	void Hit(Robot& target);
};

class Building {
public:
	std::string Type_;
	int Health_;
	bool Invincible;
	bool Protected;

	Building(std::string type, int health);

	bool Survive()const;

};



#endif // ROBOT_H
