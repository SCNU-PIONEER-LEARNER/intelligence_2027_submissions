#include <iostream>
#include <string>
using namespace std;

int main() {
    int a = 42;
    float f = 3.14f;
    double d = 3.141592653;
    char c = 'A';
    string s = "PIONEER!";
    int* p = nullptr; // 空指针，不指向任何东西

    // 用 sizeof 看每种类型占多少字节
    cout << a << " 是 int，占 " << sizeof(int) << " 字节" << endl;
    cout << f << " 是 float，占 " << sizeof(float) << " 字节" << endl;
    cout << d << " 是 double，占 " << sizeof(double) << " 字节" << endl;
    cout << c << " 是 char，占 " << sizeof(char) << " 字节" << endl;
    cout << s << " 是 string" << endl;
    cout << "p 的值是 " << p << endl;

    // 思考题：
    // 1. 字节是存储的基本单位，1 字节 = 8 个 bit
    // 2. 不一样，char 1 字节、int/float 4 字节、double 8 字节
    // 3. 内存就是程序运行时放变量的地方，可以想成一排带编号的柜子
    // 4. 一般不会，程序启动时已经加载进内存了，删的只是磁盘上的文件
    // 5. auto 不是类型，它只是让编译器根据初始值自动推断类型

    auto x = 3.14; // 被推断成 double
    auto y = 100;  // 被推断成 int
    cout << "x 占 " << sizeof(x) << " 字节，y 占 " << sizeof(y) << " 字节" << endl;

    return 0;
}
