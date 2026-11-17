#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
class Ball
{
protected:
    double size;     // 尺寸
    double price;    // 价格
public:
    Ball(double s, double p) : size(s), price(p) {}
    virtual void showInfo()
    {
        cout << "球：尺寸=" << size << "，价格=" << price << endl;
    }
};
class Pellet : public Ball
{
private:
    string typeName; // 弹丸名称
public:
    Pellet(string name, double s, double p) : Ball(s, p), typeName(name) {}
    void showInfo() override
    {
        cout << "====获得奖励弹丸====" << endl;
        cout << "弹丸种类：" << typeName << endl;
        cout << "尺寸：" << size << endl;
        cout << "价格：" << price << endl;
        cout << "====================" << endl;
    }
};
int main()
{
    srand((unsigned int)time(nullptr)); 
    int A = 0; 
    while(A < 100)
    {
        A += 10;
        cout << "当前经验值A = " << A << endl;
    }
    int randNum = rand() % 2;
    if(randNum == 0)
    {
        Pellet bigPellet("大弹丸", 12.5, 50.0);
        bigPellet.showInfo();
    }
    else
    {
        Pellet smallPellet("小弹丸", 6.0, 20.0);
        smallPellet.showInfo();
    }
    return 0;
}
