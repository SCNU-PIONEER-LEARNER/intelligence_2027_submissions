// 1.6.2 使用 vector 存 1~10000 中 13 的倍数
#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

int main() {
    SetConsoleOutputCP(65001);  
    vector<int> v;

    // 不需要预先知道个数
    for (int i = 13; i <= 10000; i += 13) {
        v.push_back(i);
    }

    cout << "1~10000 中 13 的倍数共 " << v.size() << " 个:" << endl;
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i] << endl;
    }
    return 0;
}
