// 1.6.1 使用数组存 1~10000 中 13 的倍数
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    SetConsoleOutputCP(65001);  
    // 1~10000 中 13 的倍数最多有 10000 / 13 = 769 个
    int CAP = 10000 / 13 ;
    int arr[CAP];
    int n = 0;  // 实际存入的个数

    for (int i = 13; i <= 10000; i += 13) {
        arr[n++] = i;
    }

    cout << "1~10000 中 13 的倍数共 " << n << " 个:" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << endl;
    }
    return 0;
}
