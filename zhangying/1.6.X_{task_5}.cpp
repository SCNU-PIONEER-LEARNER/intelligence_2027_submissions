#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;
int zhishu(int n)
{
    if(n<=1)
       return 0;
    for(int j=2; j*j<=n; j++)
    {
        if (n%j==0)
         {
            return 0;
        }
    }
    return 1;
}
int main()
{
    SetConsoleOutputCP(65001);
    vector<int>v1;//v1为单数
    vector<int>v2;//v2为双数
    vector<int>v3;//v3为质数
    for(int i=1; i<=100; i++)
    {
        if(i%2==1)
        {
            v1.push_back(i);
        }
        else
        {
            v2.push_back(i);
        }
        if(zhishu(i))
        {
            v3.push_back(i);
        }
    }
    for(int num:v3)
    {
        if(num%2==1)
        {
            cout << "同时是单数和质数："  << num <<" "<<endl;
        }
    }
    return 0;
    //vector的容量是动态的，可以动态扩展，而arry数组的大小是固定的
    //任务一没有引入数组
    //可以用vector优化代码
}
