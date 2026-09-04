#include <cstdio>    // printf 所在的标准头文件（C 风格）
#include <iostream>  // cin / cout 所在的标准头文件（C++ 风格）
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
    // ===== 防乱码：让 Windows 控制台按 UTF-8 显示中文 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 1. 使用 printf 输出 "PIONEER! "
    printf("PIONEER! ");

    // 2. 使用 iostream（cout）输出 "PIONEER! "
    std::cout << "PIONEER! ";

    // 3. printf 与 iostream 的区别（面试常问）：
    //    - printf 来自 C 语言，是“格式化输出”，需要自己写格式占位符 %d、%f、%s 等，
    //      占位符和参数类型不匹配时编译器不会报错，容易踩坑；
    //    - cout 来自 C++，通过运算符重载自动识别变量类型，写起来更安全、更简洁，
    //      也更符合 C++ 面向对象的风格；
    //    - 性能上 printf 通常更快，但现代 C++ 项目更推荐使用 cout / 流式输出。
    std::cout << std::endl;

    return 0;
}
