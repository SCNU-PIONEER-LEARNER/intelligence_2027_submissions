#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "../include/Robot.h"

enum Color
{
    RED,
    BLUE,
};

enum Target
{
    BUILDINGS,
    ROBOTS,
};


int main()
{
    //生成一个种子用于随机数的生成
    auto seed = std::time(nullptr);
    std::srand(static_cast<unsigned int>(seed));

    //使用vector容器存放红蓝双方机器人及建筑物
    //这里四个参数分别为 机器人类型 生命值 攻击力 命中率
    auto guard = Robot("Guard", 600, 5, 0.7);       //0哨兵
    auto infantry = Robot("Infantry", 200, 5, 0.8); //1步兵
    auto hero = Robot("Hero", 250, 100, 0.5);       //2英雄
    std::vector<Robot> half_robots = {guard, infantry, hero};
    std::vector<decltype(half_robots)> robots(2, half_robots);

    //参数为 建筑物类型 生命值
    auto outpost = Building("Outpost", 2000); //0前哨站
    auto base = Building("Base", 5000);       //1基地
    std::vector<Building> half_buildings = {outpost, base};
    std::vector<decltype(half_buildings)> buildings(2, half_buildings);

    int i = 0;

    while (1)
    {
        //前哨站未被摧毁时，基地处于无敌状态。
        buildings[RED][1].Invincible = buildings[RED][0].Survive();
        buildings[BLUE][1].Invincible = buildings[BLUE][0].Survive();

        //哨兵存活时，基地处于保护状态。
        buildings[RED][1].Protected = robots[RED][0].Survive();
        buildings[BLUE][1].Protected = robots[BLUE][0].Survive();

        //随机决定进攻方和进攻对象。
        Color color = static_cast<Color>(std::rand() % 2);
        Target target = static_cast<Target>(std::rand() % 2);

        //使用引用取得真正的机器人对象，避免产生副本。
        auto &attacker = robots[color][std::rand() % 3];

        if (target == BUILDINGS)
        {
            auto &building_target = buildings[!color][std::rand() % 2];
            attacker.Hit(building_target);
        }
        else
        {
            auto &robot_target = robots[!color][std::rand() % 3];
            attacker.Hit(robot_target);
        }

        //胜利判断条件
        if (!(buildings[BLUE][1].Survive()))
        {
            std::cout << "Blue base destroyed. Red wins!" << std::endl;
            break;
        }
        else if (!(buildings[RED][1].Survive()))
        {
            std::cout << "Red base destroyed. Blue wins!" << std::endl;
            break;
        }
        
        i++;
        //每循环100次输出当前状态
        if (i % 100 == 0)
        {
            std::cout << "Current round: " << i << std::endl;
            std::cout << "B_Outpost HP:" << buildings[BLUE][0].Health_ << std::endl;
            std::cout << "R_Outpost HP:" << buildings[RED][0].Health_ << std::endl;
            std::cout << "R_Guard HP:" << robots[RED][0].Health_ << std::endl;
            std::cout << "B_Guard HP:" << robots[BLUE][0].Health_ << std::endl;
            std::cout << "R_Base HP:" << buildings[RED][1].Health_ << std::endl;
            std::cout << "B_Base HP:" << buildings[BLUE][1].Health_ << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}
