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

    std::vector<int> nums;
    for (int i = 1; i <= 100; ++i)
        nums.push_back(i);

    // 遍历删除所有奇数：erase 返回下一个有效迭代器，注意迭代器不能随便 ++
    for (auto it = nums.begin(); it != nums.end();)
    {
        if (*it % 2 == 1)
            it = nums.erase(it);
        else
            ++it;
    }

    std::cout << "删除所有单数后，剩余 " << nums.size() << " 个双数：" << std::endl;
    for (int n : nums)
    {
        std::cout << n << " ";
        if (n % 20 == 0)   // 每 10 个数换行
            std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
