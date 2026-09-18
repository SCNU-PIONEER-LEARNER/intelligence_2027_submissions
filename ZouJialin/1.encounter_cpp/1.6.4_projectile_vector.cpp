#include<iostream>
#include<string>
#include<vector>
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
    vector<danwan> v;
    v.push_back(danwan(42, 15));
    v.push_back(danwan(17, 1));
    v.push_back(danwan(42, 15));
    v.push_back(danwan(17, 1));
     for(int i=0;i<v.size();i++)
        cout<<"第"<<i + 1<<"个是"<<v[i].gettype()
             <<"（直径"<<v[i].getdiameter()<<"mm）"<<endl;
    return 0;
}