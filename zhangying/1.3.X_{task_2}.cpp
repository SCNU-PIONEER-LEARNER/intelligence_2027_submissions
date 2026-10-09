#include<windows.h>
#include <iostream>
using namespace std;
int main()
{
SetConsoleOutputCP(65001);
cout <<"请输入一个数字"<< endl;
int a=0;
cin >> a;
int flag=a>60?1:0;
switch(flag)
{
case 1:
cout <<"合格"<< endl;
break;
case 0:
cout <<"不合格"<< endl;
}
}