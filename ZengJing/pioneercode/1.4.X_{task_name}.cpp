// 1. 写⼀个传值调⽤的加法函数，并将其使⽤在任务⼀。
// 2. 写⼀个引⽤调⽤的加法函数，并将其使⽤在任务⼀。
#include <iostream>
using namespace std;
double add(double a,double b)
{
    return a+b;
}
double fadd(double a,double b,double& x)
{
    x=a+b;
    return a+b;
}
int main()
{
    double a=1.1;
    double b=2.2;
    double x=0.0;
    cout<<add(a,b)<<endl;
    cout<<"a="<<a<<" b="<<b<<" x="<<x<<endl;
    cout<<fadd(a,b,x)<<endl;
    cout<<"a="<<a<<" b="<<b<<" x="<<x<<endl;
    return 0;
}