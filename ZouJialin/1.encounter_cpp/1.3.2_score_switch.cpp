#include<iostream>
using namespace std;

int main()
{
    int score;
    cout<<"请输入分数：";
    cin>>score;

    switch (score/10)
    {
        case 10:
        case 9:
        case 8:
        case 7:
        case 6:
        cout<<"合格"<<endl;
        break;
        default:
        cout<<"不合格"<<endl;
    }
    return 0;
}