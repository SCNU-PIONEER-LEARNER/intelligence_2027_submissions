#include<iostream>
#include<random>
#include<String>
using namespace std;

class Ball{
public:
    Ball(int D):m_D(D){}
    int m_D;
};

class Pellet:public Ball{
public:
    Pellet(int D,int price):Ball(D),m_price(price){}
    string getPelletProperties(){
        return "价格：" + to_string(m_price) + " 直径：" + to_string(m_D);
    }
public:
    int m_price;
};

int main(){

    Pellet BigPellet(42,10);
    Pellet SmallPellet(17,1);

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
            float En = pre_E1 + pre_E2;
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
    int ballmode = dist(gen);
    if(ballmode){
        cout << "奖励大球 " << BigPellet.getPelletProperties() << endl;
    }else{
        cout << "奖励小球 " << SmallPellet.getPelletProperties() << endl;
    }
    

    return 0;
}