#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

// 传值调用的加法函数
int add(int a, int b){
    return a + b;
}

int main(){
    SetConsoleOutputCP(65001);
    srand(time(0));

    int day = 0;    // 天数
    int state = 0;  // 当天的状态：0-视觉高手，1-打瓦高手
    int A = 0;      // 学习经验累积值
    int e1 = 0;     // 上一次处于状态(1)时获得的经验 E(n-1)
    int e2 = 1;     // 再上一次处于状态(1)时获得的经验 E(n-2)
                    // 初始设为 0 和 1，使经验序列为 1,1,2,3,5,8...（标准斐波那契）
    int eNow = 0;   // 当天经验变化量

    while (A < 100){
        day++;
        state = rand() % 2;

        if (state == 1){
            // 状态(1)：获得学习经验 E(n) = E(n-1) + E(n-2)，用传值调用计算
            eNow = add(e1, e2);
            e2 = e1;
            e1 = eNow;
            A += eNow;
            cout << "第" << day << "天：状态(1) 打瓦高手，获得经验 " << eNow
                 << "，累积经验 A=" << A << endl;
        } else {
            // 状态(0)：减少经验 L(n) = E(L)/2
            eNow = -(e1 / 2);
            A += eNow;
            if (A < 0){
                A = 0;
            }
            cout << "第" << day << "天：状态(0) 视觉高手，减少经验 " << -eNow
                 << "，累积经验 A=" << A << endl;
        }
    }

    cout << "YOU ARE WELCOME TO JOIN PIONEER!!" << endl;
    return 0;
}
