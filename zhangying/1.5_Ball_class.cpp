#include <iostream>
#include <random>
#include <string>
#include <windows.h>
using namespace std;
class ball
{
    public:
    ball(double r):radius(r){}
    double getradius()
    {
        return radius;
    }
    private:
    double radius;
};
class bullet:public ball
{
    public:
    bullet(double r,string t,string m,double p)
    :ball(r),type(t),mode(m),price(p){}
    void show()
    {
        cout<<"弹丸的类型"<< type
            <<"直径："<< 2*getradius()<<"mm"
            <<"兑换方式："<< mode
            <<"单发价钱："<< price;       
    }
    private:
    double price;
    string mode;
    string type;
};
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
    uniform_real_distribution<double> d(0.0,1.0);
    double r1=d(gen);
    double r2=d(gen);
    if(r1>0.5){
        if(r2>0.5){
        bullet (21,"大弹丸","远程兑换",15.0).show();
        }else{
        bullet(21,"大弹丸","非远程兑换",10.0).show();
        }
    }else{
        if(r2>0.5){
        bullet (8.5,"小弹丸","远程兑换",1.5).show();
        }else{
        bullet(8.5,"小弹丸","非远程兑换",1.0).show();
        }
    }  
}