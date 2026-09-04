#include <iostream>
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

    int count = 0;
    std::cout << "1~10000 中 13 的倍数：" << std::endl;

    for (int i = 1; i <= 10000; ++i)
    {
        if (i % 13 == 0)   // 能被 13 整除
        {
            std::cout << i << " ";
            ++count;
            if (count % 20 == 0)   // 每行输出 20 个，方便查看
                std::cout << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "共 " << count << " 个" << std::endl;
    return 0;
}
