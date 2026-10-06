#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
    // 1. 使用 printf 输出 "PIONEER!"
    printf("PIONEER!\n");

    // 2. 使用 iostream（cout）输出 "PIONEER!"
    cout << "PIONEER!" << endl;

    // 3. printf 与 iostream 的简单区别：
    //    - printf 来自 C 标准库 <cstdio>，使用格式字符串控制输出，效率通常更高。
    //    - cout 来自 C++ 标准库 <iostream>，支持类型安全、可扩展（运算符重载），更面向对象。

    return 0;
}
