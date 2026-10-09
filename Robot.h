#include <ctime>
#include <cstdlib>
#include <string>
class robot_target
{
    public:
    Robot(std::string name, int hp, int atk, double hit_rate; )
    bool Survive ();
    void Hit(Robot &target);
    void Hit(class Building &target);
    int Health;
    std::string Name;
    int Attack;
    double HitRate;
};
class Building
{
    public:
    Building(std::string name, int hp);
    boll Survive();
    int Health;
    std::string Name;
    bool Invincible;
    bool Protected;
}