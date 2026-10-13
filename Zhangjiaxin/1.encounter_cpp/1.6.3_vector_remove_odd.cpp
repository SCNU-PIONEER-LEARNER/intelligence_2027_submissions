// 1.6.3 使用 vector 存 1~100, 然后删除所有单数(奇数)
#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    vector<int> v;
    for (int i = 1; i <= 100; i++) {
        v.push_back(i);
    }

    for (vector<int>::iterator it = v.begin(); it != v.end(); ) {
        if (*it % 2 != 0)
            it = v.erase(it);  // 删除单数
        else
            ++it;
    }

    cout << "删除单数后剩 " << v.size() << " 个偶数:" << endl;
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i] << endl;
    }
    return 0;
}
