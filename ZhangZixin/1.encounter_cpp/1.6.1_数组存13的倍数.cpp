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

    // 1~10000 中 13 的倍数最多 10000/13 ≈ 769 个，预留 800 足够
    int arr[800] = {0};
    int count = 0;

    for (int i = 1; i <= 10000; ++i)
    {
        if (i % 13 == 0)
        {
            arr[count] = i;   // 存入数组
            ++count;
        }
    }

    std::cout << "数组中共存储 " << count << " 个 13 的倍数：" << std::endl;
    for (int j = 0; j < count; ++j)
    {
        std::cout << arr[j] << " ";
        if ((j + 1) % 20 == 0)   // 每行 20 个
            std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
