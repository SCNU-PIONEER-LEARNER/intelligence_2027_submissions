// 1.6.5_vector_odd_even_prime.cpp —— 三个 vector 存单数/双数/质数，输出"既是单数又是质数"的数量
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> odds, evens, primes;

    for (int i = 1; i <= 100; i++) {
        if (i % 2 != 0) odds.push_back(i);   // 单数（奇数）
        else            evens.push_back(i);  // 双数（偶数）

        // 质数判定：从 2 试除到 sqrt(i)，有因子就不是质数
        bool isPrime = i >= 2;
        for (int d = 2; d * d <= i; d++) {
            if (i % d == 0) { isPrime = false; break; }
        }
        if (isPrime) primes.push_back(i);
    }

    cout << "1~100 中：单数 " << odds.size() << " 个，双数 " << evens.size()
         << " 个，质数 " << primes.size() << " 个" << endl;

    // 既是单数又是质数 = 质数里去掉唯一的偶质数 2
    int count = 0;
    for (int p : primes) {
        if (p % 2 != 0) count++;
    }
    cout << "既是单数又是质数的数共 " << count << " 个（就是除 2 以外的全部质数）" << endl;
    return 0;
}
