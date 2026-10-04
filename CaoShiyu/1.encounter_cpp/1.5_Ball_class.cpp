#include <iostream>
#include <random>
#include <string>

class Ball
{
protected:
    int diameter;

public:
    Ball(int ballDiameter) : diameter(ballDiameter)
    {
    }
};

class Projectile : public Ball
{
private:
    std::string name;
    int price;

public:
    Projectile(std::string projectileName, int projectileDiameter, int projectilePrice)
        : Ball(projectileDiameter), name(projectileName), price(projectilePrice)
    {
    }

    void showInfo() const
    {
        std::cout << "Reward: " << name << std::endl;
        std::cout << "Diameter: " << diameter << " mm" << std::endl;
        std::cout << "Price: " << price << " coins" << std::endl;
    }
};

int main()
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> state(0, 1);

    int total = 0;
    int previousExp = 1;
    int currentExp = 1;
    int lastGain = 1;
    int day = 1;

    Projectile smallProjectile("Small projectile", 17, 10);
    Projectile largeProjectile("Large projectile", 42, 100);

    while (total < 100)
    {
        int todayState = state(engine);

        if (todayState == 0)
        {
            int todayExp = previousExp + currentExp;
            total += todayExp;

            previousExp = currentExp;
            currentExp = todayExp;
            lastGain = todayExp;

            std::cout << "Day " << day
                      << ": Learn computer vision, gain "
                      << todayExp << "exp." << std::endl;
        }
        else
        {
            int loss = lastGain / 2;
            total -= loss;
            if (total < 0)
            {
                total = 0;
            }

            std::cout << "Day " << day
                      << ": Play VALORANT, lose "
                      << loss << "exp." << std::endl;
        }

        std::cout << "Total exp: " << total << std::endl;
        day++;
    }

    std::cout << "YOU ARE WELCOME TO JOIN PIONEER!" << std::endl;

    std::uniform_int_distribution<int> rewardDistribution(0, 1);

    if (rewardDistribution(engine) == 0)
    {
        smallProjectile.showInfo();
    }
    else
    {
        largeProjectile.showInfo();
    }

    return 0;
}