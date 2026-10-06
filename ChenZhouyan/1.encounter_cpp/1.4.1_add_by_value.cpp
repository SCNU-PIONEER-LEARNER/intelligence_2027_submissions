#include <iostream>
using namespace std;

// 传值调用的加法函数：形参 x、y 是实参的一份拷贝，
// 函数里怎么改都不会影响到外面的变量
int add(int x, int y) {
    return x + y;
}

int main() {
    int a = 3, b = 5;
    cout << a << " + " << b << " = " << add(a, b) << endl;

    // 换一组数再试一次
    a = 10;
    b = -2;
    cout << a << " + " << b << " = " << add(a, b) << endl;

    return 0;
}
