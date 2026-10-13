#include <iostream>
#include <windows.h>
using namespace std;

int main(){
    SetConsoleOutputCP(65001);
    
    int score = 0;
    int n = 0;

    cout <<"请输入你的分数：";
    cin >> score;
    n = score >= 60 ? 1 : 0;

    switch(n){
        case 1:
            cout <<"及格"<<endl;
            break;
        case 0:
            cout <<"不及格"<<endl;
            break;
    }

    return 0;
}