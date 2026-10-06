// 1.6.1_array_multiples_of_13.cpp —— 用数组存 1~10000 中 13 的倍数
#include <iostream>
using namespace std;

int main() {
    // 10000/13=769 个，数组开 770 绰绰有余（数组大小必须编译期确定，宁大勿小）
    int arr[770];
    int count = 0;

    for (int i = 13; i <= 10000; i += 13) {
        arr[count++] = i;  // count++：先用在加，正好当存放下标
    }

    for (int i = 0; i < count; i++) {
        cout << arr[i] << " ";
        if ((i + 1) % 10 == 0) cout << endl;  // 每行 10 个，好看
    }
    cout << endl << "数组共存了 " << count << " 个 13 的倍数" << endl;
    return 0;
}
