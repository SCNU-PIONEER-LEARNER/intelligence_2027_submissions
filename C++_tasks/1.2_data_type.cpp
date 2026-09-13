#include <iostream>
#include <string>
using namespace std;

int main() {
int a = 10;            //整数
float b = 3.14f;   //单精度浮点数
double c = 3.1415926;   //双精度浮点数
char d = 'A';   //单个字符
string e = "PIONEER";     //字符串
int *p = nullptr;  //空指针

cout << "int大小" << sizeof(a) << "字节" << endl;
cout << "float大小" << sizeof(b) << "字节" << endl;
cout << "double大小" << sizeof(c) << "字节" << endl;
cout << "char大小" << sizeof(d) << "字节" << endl;
cout << "string大小" << sizeof(e) << "字节" << endl;
cout << "指针大小" << sizeof(p) << "字节" << endl;

return 0;
}