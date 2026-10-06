#include <iostream>
#include <random>
using namespace std;

// 斐波那契数列小游戏
// 规则：每天随机处于两种状态，1 = 打瓦高手，当天获得经验 E(n) = E(n-1) + E(n-2)
//       0 = 视觉高手，当天损失上一次打瓦日所得经验的一半，即 E(L)/2
//       累积经验 A 到 100 就输出 "YOU ARE WELCOME TO JOIN PIONEER!"
// 经验从 E(1)=1、E(2)=1 起步，A 最低扣到 0

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dailyState(0, 1); // 每天 0 或 1，各一半

    long long A = 0;        // 累积经验
    long long ePrev1 = 1;   // E(n-1)
    long long ePrev2 = 0;   // E(n-2)
    long long lastGain = 0; // 上一次打瓦日获得的经验 E(L)
    int day = 0;

    while (A < 100) {
        day++;
        int state = dailyState(gen);

        if (state == 1) {
            long long gain = ePrev1 + ePrev2; // 斐波那契式增长
            A += gain;
            lastGain = gain;
            ePrev2 = ePrev1;
            ePrev1 = gain;
            cout << "第" << day << "天 打瓦高手，经验 +" << gain << "，A = " << A << endl;
        } else {
            long long loss = lastGain / 2;
            A -= loss;
            if (A < 0) A = 0;
            cout << "第" << day << "天 视觉高手，经验 -" << loss << "，A = " << A << endl;
        }
    }

    cout << "用了 " << day << " 天" << endl;
    cout << "YOU ARE WELCOME TO JOIN PIONEER!" << endl;

    return 0;
}
