#include <iostream>
#include <random>
#ifdef _WIN32
#include <windows.h>
#endif

// 引用调用的加法函数：直接修改传入的实参 a，使 a = a + b
void addByReference(double &a, double b)
{
    a = a + b;
}

int main()
{
    // ===== 防乱码 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 演示：引用调用会直接修改实参
    double x = 3.0, y = 5.0;
    addByReference(x, y);
    std::cout << "调用 addByReference 后 x = " << x << "（实参已被直接改成 3+5=8）" << std::endl << std::endl;

    // 任务一游戏：用引用加法累加 / 扣减经验
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
                last_E = 1.0;                    // E(1)=E(2)=1
            else
                last_E = e1 + e2;                // E(n)=E(n-1)+E(n-2)
            e1 = e2;
            e2 = last_E;
            addByReference(A, last_E);           // 用引用加法直接累加经验
            std::cout << "我想成为视觉高手（0），获得经验 " << last_E;
        }
        else
        {
            double loss = last_E / 2.0;          // L(n) = E(L)/2
            addByReference(A, -loss);            // 用引用加法直接扣减经验
            if (A < 0)
                A = 0;
            std::cout << "我想成为打瓦高手（1），减少经验 " << loss;
        }

        std::cout << "，当前经验 A = " << A << std::endl;
    }

    std::cout << "\"YOU ARE WELCOME TO JOIN PIONEER!\"" << std::endl;
    return 0;
}
