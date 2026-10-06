// 1.6.3_vector_remove_odd.cpp —— vector 存 1~100，删掉所有单数（奇数）
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;
    for (int i = 1; i <= 100; i++) v.push_back(i);

    // 初学者写法：再开一个 vector，把双数（偶数）一个个挑出来
    vector<int> even;
    int n = v.size();
    for (int i = 0; i < n; i++) {
        if (v[i] % 2 == 0) {   // 能被 2 整除的就是双数
            even.push_back(v[i]);
        }
    }
    v = even;  // 用只剩双数的新 vector 替换原来的

    cout << "删完单数后剩 " << v.size() << " 个（应该正好是 50 个双数）：" << endl;
    for (int x : v) cout << x << " ";
    cout << endl;
    return 0;
}
