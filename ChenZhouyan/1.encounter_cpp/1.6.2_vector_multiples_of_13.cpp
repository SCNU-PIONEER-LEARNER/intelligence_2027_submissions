// 1.6.2_vector_multiples_of_13.cpp —— 用 vector 存 1~10000 中 13 的倍数
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;  // 不用提前算容量，push_back 自动扩容，这是和数组最大的区别

    for (int i = 13; i <= 10000; i += 13) {
        v.push_back(i);
    }

    // 用范围 for 遍历，比下标版更简洁
    int line = 0;
    for (int x : v) {
        cout << x << " ";
        if (++line % 10 == 0) cout << endl;
    }
    cout << endl << "vector 共存了 " << v.size() << " 个 13 的倍数" << endl;
    return 0;
}
