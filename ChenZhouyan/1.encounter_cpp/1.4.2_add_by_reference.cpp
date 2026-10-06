#include <iostream>
using namespace std;

// 引用调用的加法函数：result 是外部变量的别名（引用），
// 函数里给它赋值，就是直接改外面的变量
// 对比传值调用：这里不用 return，结果直接写进了 sum
void add(int x, int y, int& result) {
    result = x + y;
}

int main() {
    int a = 3, b = 5, sum = 0;
    add(a, b, sum);
    cout << a << " + " << b << " = " << sum << endl;

    a = 10;
    b = -2;
    add(a, b, sum);
    cout << a << " + " << b << " = " << sum << endl;

    return 0;
}
