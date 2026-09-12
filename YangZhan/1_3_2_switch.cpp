//输⼊⼀个分数，⼤于60输出“合格”，⼩于60输出“不合格”
#include <iostream>
using namespace std;

int main(){
    int score;
    cout<<"请输入一个分数：";
    cin>>score;
    switch(score/10){
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            cout<<"不合格"<<endl;
            break;
        default:
            cout<<"合格"<<endl;
            break;
    }
}
