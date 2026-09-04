#include <iostream>
#include <vector>
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

    std::vector<int> multiples;
    for (int i = 1; i <= 10000; ++i)
    {
        if (i % 13 == 0)
            multiples.push_back(i);   // 动态追加，不用预先算好大小
    }

    std::cout << "vector 中共存储 " << multiples.size() << " 个 13 的倍数：" << std::endl;
    for (size_t j = 0; j < multiples.size(); ++j)
    {
        std::cout << multiples[j] << " ";
        if ((j + 1) % 20 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
