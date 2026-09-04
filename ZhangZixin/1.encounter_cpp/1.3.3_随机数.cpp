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

    // random_device：尽量使用硬件熵源生成“真随机”种子
    std::random_device rd;
    // mt19937：梅森旋转伪随机数生成器（C++11 <random> 库的常用引擎）
    std::mt19937 gen(rd());
    // uniform_int_distribution：均匀分布在 [0, 30] 的整数
    std::uniform_int_distribution<int> dist(0, 30);

    int num = dist(gen);
    std::cout << "0~30 之间的随机数：" << num << std::endl;
    return 0;
}
