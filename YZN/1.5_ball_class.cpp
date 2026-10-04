#include <cstdio>
#include <cstdlib>
#include <ctime>

class Ball
{
private:
    double banjing; 
public:
    Ball(double r)
    {
        banjing = r;
    }
    double getBanJing()
    {
        return banjing;
    }
};

class Pellet : public Ball
{
private:
    int jiage; 
public:
    Pellet(double r, int p) : Ball(r)
    {
        jiage = p;
    }
    void printInfo()
    {
        printf("Pellet radius: %.2f\n", getBanJing());
        printf("Pellet price: %d\n", jiage);
    }
};

int main()
{
    srand((unsigned int)time(NULL));

    double A = 0;
    double num1 = 1;
    double num2 = 1;
    double temp = 1;

    // 积累经验
    while (A < 100)
    {
        int zhuangtai = rand() % 2;
        if (zhuangtai == 1)
        {
            double now = num1 + num2;
            A = A + now;
            temp = now;
            num2 = num1;
            num1 = now;
        }
        else
        {
            A = A - temp / 2;
        }
    }

    printf("Congratulations! Experience full!\n");

    int xuanze = rand() % 2;
    if (xuanze == 0)
    {
        Pellet xiao(3, 20);
        xiao.printInfo();
    }
    else
    {
        Pellet da(8, 50);
        da.printInfo();
    }

    return 0;
}