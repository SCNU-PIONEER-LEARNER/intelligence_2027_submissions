#include <iostream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

// 判断一个数是否为质数
bool isPrime(int n)
{
    if (n < 2)
        return false;
    for (int i = 2; i * i <= n; ++i)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main()
{
    // ===== 防乱码 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::vector<int> odds;     // 单数
    std::vector<int> evens;    // 双数
    std::vector<int> primes;   // 质数

    for (int i = 1; i <= 100; ++i)
    {
        if (i % 2 == 1)
            odds.push_back(i);
        else
            evens.push_back(i);
        if (isPrime(i))
            primes.push_back(i);
    }

    std::cout << "单数（" << odds.size() << " 个）：";
    for (int n : odds)
        std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "双数（" << evens.size() << " 个）：";
    for (int n : evens)
        std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "质数（" << primes.size() << " 个）：";
    for (int n : primes)
        std::cout << n << " ";
    std::cout << std::endl;

    // 同时是单数和质数的数值（质数中除 2 外都是奇数，所以这里就是所有奇数质数）
    std::cout << "同时是单数和质数的数值：";
    for (int n : primes)
    {
        if (n % 2 == 1)
            std::cout << n << " ";
    }
    std::cout << std::endl;
    return 0;
}
