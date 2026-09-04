#include <iostream>
#include <random>
#ifdef _WIN32
#include <windows.h>
#endif

// 传值调用的加法函数：返回 a + b，不修改实参
double addByValue(double a, double b)
{
    return a + b;
}

int main()
{
    // ===== 防乱码 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 演示：传值调用不会改变实参
    double x = 3.0, y = 5.0;
    double result = addByValue(x, y);
    std::cout << "addByValue(" << x << ", " << y << ") = " << result << std::endl;
    std::cout << "传值调用后 x、y 不变：x = " << x << "，y = " << y << std::endl << std::endl;

    // 任务一游戏：用传值加法计算斐波那契经验并累加 A
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    double A = 0.0;
    double last_E = 0.0;
    double e1 = 0.0, e2 = 0.0;
    int state0_count = 0;
    int day = 0;

    while (A < 100)
    {
        ++day;
        int state = dist(gen);
        std::cout << "第 " << day << " 天：";

        if (state == 0)
        {
            ++state0_count;
            if (state0_count <= 2)
                last_E = 1.0;                                  // E(1)=E(2)=1
            else
                last_E = addByValue(e1, e2);                   // E(n)=E(n-1)+E(n-2)
            e1 = e2;
            e2 = last_E;
            A = addByValue(A, last_E);                         // 用传值加法累加经验
            std::cout << "我想成为视觉高手（0），获得经验 " << last_E;
        }
        else
        {
            double loss = last_E / 2.0;                        // L(n) = E(L)/2
            A = addByValue(A, -loss);                          // 用传值加法扣减经验
            if (A < 0)
                A = 0;
            std::cout << "我想成为打瓦高手（1），减少经验 " << loss;
        }

        std::cout << "，当前经验 A = " << A << std::endl;
    }

    std::cout << "\"YOU ARE WELCOME TO JOIN PIONEER!\"" << std::endl;
    return 0;
}
