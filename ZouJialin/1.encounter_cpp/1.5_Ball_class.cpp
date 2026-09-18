#include<iostream>
#include<string>
#include<random>
using namespace std;

class ball
{
    private:
    double diameter;
    double price;

    public:
    ball(double d,double p)
    {
        diameter=d;
        price=p;
    }
    
    double getdiameter(){return diameter;}
    double getprice(){return price;}
};

class danwan:public ball
{
    public:
    danwan(double d,double p):ball(d,p){}
    string gettype()
    {
        if(getdiameter()==42) return"大弹丸";
        else return "小弹丸";
    }
};
int main()
{
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<int> t(0,1);
    int A=0,a=1,b=1,next=0,yesterday=0;
    while(A<100)
    {
        int feel=t(gen);
        if(feel==0)
        {
            A+=a;
            yesterday=a;
            next=a+b;
            a=b;
            b=next;
        }
        else
        {
            A-=yesterday/2;
        }
    }
    cout<<"YOU ARE WELCOME TO JOIN PIONEER!"<<endl;

    int k=t(gen);
    if(k==0)
    {
        danwan m(42,15);
        cout<<"获得一个"<<m.gettype()<<",价格是"<<m.getprice()<<"金币"<<endl;
    }
    else
    {
        danwan m(17,1);
        cout<<"获得一个"<<m.gettype()<<",价格是"<<m.getprice()<<"金币"<<endl;
    }
    return 0;
}