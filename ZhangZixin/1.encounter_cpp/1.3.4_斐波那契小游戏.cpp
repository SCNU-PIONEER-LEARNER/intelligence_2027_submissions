#include <iostream>
#include <random>
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
    // ===== 防乱码 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);  // 每天随机 0 或 1

    double A = 0.0;        // 学习经验累积值
    double last_E = 0.0;   // 上一次处于状态 0 时获得的经验 E(L)
    double e1 = 0.0;       // 斐波那契滚动变量 E(n-1)
    double e2 = 0.0;       // 斐波那契滚动变量 E(n)
    int state0_count = 0;  // 进入状态 0 的次数（决定斐波那契第几项）
    int day = 0;           // 经过的天数

    // while 循环：经验没到 100 就一直“过日子”
    while (A < 100)
    {
        ++day;
        int state = dist(gen);   // 当天随机状态
        std::cout << "第 " << day << " 天：";

        // switch 根据状态分派奖励 / 惩罚
        switch (state)
        {
        case 0:  // 视觉高手：获得斐波那契经验
        {
            ++state0_count;
            if (state0_count == 1)
                last_E = 1.0;                       // E(1) = 1
            else if (state0_count == 2)
                last_E = 1.0;                       // E(2) = 1
            else
                last_E = e1 + e2;                   // E(n) = E(n-1) + E(n-2)
            e1 = e2;
            e2 = last_E;
            A += last_E;                            // 累加经验
            std::cout << "我想成为视觉高手（0），获得经验 " << last_E;
            break;
        }
        case 1:  // 打瓦高手：扣掉上一次视觉高手经验的一半
        {
            double loss = last_E / 2.0;             // L(n) = E(L) / 2
            A -= loss;
            std::cout << "我想成为打瓦高手（1），减少经验 " << loss;
            if (A < 0)
                A = 0;   // 经验不为负（if 语句的应用）
            break;
        }
        default:
            break;
        }

        std::cout << "，当前经验 A = " << A << std::endl;
    }

    std::cout << "\"YOU ARE WELCOME TO JOIN PIONEER!\"" << std::endl;
    return 0;
}
