#include <iostream>
#include <random>
#include <windows.h>
using namespace std;
double add1(double a, double b)
{
   return a+b;
}
int main()
{
    SetConsoleOutputCP(65001);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,1);

    double A=0.0;
    double En_prev1=0;
    double En_prev2=1;
    double last=0;

    int day=1;
    while(A < 100)
    {
        int state = dist(gen);
        cout << "第" << day << "天" << endl;
        if(state==0)
        {
            cout <<"我想成为视觉高手"<< endl;
            double En=add1(En_prev1,En_prev2);
            cout << "今日获得经验：" << En << endl;
            A+=En  ;
            last=En;

            En_prev2=En_prev1;
            En_prev1=En;
        }
        else
        {
            cout << "我想成为打瓦高手" << endl;
            double L=last/2.0;
            cout << "扣除经验：" <<  L << endl;
            A-=L;
        }
        cout << "当前总经验A=" << A << endl;
        day++;
    }
    cout << "YOU ARE WELCOME TO JOIN PIONEER!" << endl;
    return 0;
}