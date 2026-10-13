#include <iostream>
#include <windows.h>
using namespace std;

int main(){
    SetConsoleOutputCP(65001);

    // 整型
    short num1 = 10;
    int num2 = 20;
    long num3 = 30;
    long long num4 = 40;

    cout << "num1的值为：" << num1 << endl<< "short类型占用的字节数为：" << sizeof(short) << endl;
    cout << "num2的值为：" << num2 << endl<< "int类型占用的字节数为：" << sizeof(int) << endl;
    cout << "num3的值为：" << num3 << endl<< "long类型占用的字节数为：" << sizeof(long) << endl;
    cout << "num4的值为：" << num4 << endl<< "long long类型占用的字节数为：" << sizeof(long long) << endl;

    // 浮点型
    float num5 = 3.141f;
    double num6 = 3.14159265359;
    
    cout << "num5的值为：" << num5 << endl<< "float类型占用的字节数为：" << sizeof(float) << endl;
    cout << "num6的值为：" << num6 << endl<< "double类型占用的字节数为：" << sizeof(double) << endl;

    // 字符型
    char letter = 'a';
    cout << "letter的值为：" << letter << endl<< "char类型占用的字节数为：" << sizeof(char) << endl;

    // 字符串型
    string phrase = "Hello,World!";
    cout << "phrase的值为：" << phrase << endl<< "string类型占用的字节数为：" << sizeof(string) << endl;    

    // 布尔型
    bool flag = true;
    cout << "flag的值为：" << flag << endl<< "bool类型占用的字节数为：" << sizeof(bool) << endl;

    return 0;

}