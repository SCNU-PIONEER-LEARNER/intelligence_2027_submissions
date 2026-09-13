#include<iostream>
#include <random>
using namespace std;

float chuanzhiAdd(float a,float b){
    return a + b;
}

int main(){
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 1);

    float exp = 0;
    //默认前两天，从第三天开始运行
    float pre_E1 = 1;//第一天默认值
    float pre_E2 = 1;//第二天默认值
    float last_En = 1;

    while (exp < 100)
    {
        int mode = dist(gen);
        cout << "今天状态为" << mode << endl;
        if(mode == 1){
            float En = chuanzhiAdd(pre_E1,pre_E2);
            pre_E2 = pre_E1;
            pre_E1 = En;
            exp += En;
            last_En = En;
            cout << "获得经验" << En << endl;
        }else{
            float loss = last_En / 2;
            exp -= loss;
            if(exp < 0){
                exp = 0;
            }
            cout << "减少经验" << loss << endl;
        }
        cout << "当前经验" << exp << endl;
    }
    cout << "YOU ARE WELCOME TO JOIN PIONEER!" << endl;
    
        
    
    return 0;
}