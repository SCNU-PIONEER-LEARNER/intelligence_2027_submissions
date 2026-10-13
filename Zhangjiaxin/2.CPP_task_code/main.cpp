#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include "Robot.h"

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
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8 编码
    //生成一个种子用于随机数的生成
    auto seed = time(0);
    srand((unsigned int)seed);

    //使用vector容器存放红蓝双方机器人及建筑物
    auto guard = Robot("Guard", 600, 5, 0.7);       //0哨兵
    auto infantry = Robot("Infantry", 200, 5, 0.8); //1步兵
    auto hero = Robot("Hero", 250, 100, 0.5);       //2英雄
    std::vector<Robot> half_robots = { guard, infantry, hero };
    std::vector<decltype(half_robots)> robots(2, half_robots);

    //参数为 建筑物类型 生命值
    auto outpost = Building("Outpost", 2000); //0前哨站
    auto base = Building("Base", 5000);       //1基地
    std::vector<Building> half_buildings = { outpost, base };
    std::vector<decltype(half_buildings)> buildings(2, half_buildings);

    int i = 0;

    while (1)
    {
        //基地无敌机制
        buildings[RED][1].Invincible = buildings[RED][0].Survive();
        buildings[BLUE][1].Invincible = buildings[BLUE][0].Survive();

        //基地护盾机制
        buildings[RED][1].Protected = robots[RED][0].Survive();
        buildings[BLUE][1].Protected = robots[BLUE][0].Survive();

        //决定进攻方和进攻对象
        Color color = static_cast<Color>(rand() % 2);
        Target target = static_cast<Target>(rand() % 2);

        //进攻方随机生成
        auto attacker = robots[color][rand() % 3];

        //判断进攻对象
        if (target == BUILDINGS)
        {
            auto& building_target = buildings[!color][rand() % 2];
            attacker.Hit(building_target);
        }
        else
        {
            auto& robot_target = robots[!color][rand() % 3];
            attacker.Hit(robot_target);
        }

        //胜利判断条件
        if (!(buildings[BLUE][1].Survive()))
        {
            std::cout << "蓝方基地摧毁，红方获胜" << std::endl;
            break;
        }
        else if (!(buildings[RED][1].Survive()))
        {
            std::cout << "红方基地摧毁，蓝方获胜" << std::endl;
            break;
        }

        i++;
        //每循环100次输出当前状态
        if (i % 100 == 0)
        {
            std::cout << "当前循环次数为" << i << std::endl;
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