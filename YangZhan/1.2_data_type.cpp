#include <cstdio>
#include <iostream>
using namespace std;

int a=1;
float b=2.5;
float d=4.55134193513412f; //float可以精确记住7位，占4个字节
double c=3.551453214125;//double可以精确记住15位，占8个字节
//但是printf固定输出6位小数，cout输出6位有效数字
char e='A';//char单引号，string双引号，str占据的字节多
char s[]="Hello, World!";//可以不填数字。[10]表示数组的大小，10表示数组中可以存放10个字符，字符串的结尾有一个'\0'，所以实际能存放9个字符。空格，符号都算字符。
auto g=1.23;//auto自动推导类型，g为double类型

//指针
int f=15;
int* p=&f;//指针变量p存放的是变量f的地址，&取地址符，*取值符

int* q=nullptr;//空指针，指针变量q存放的是nullptr,可以用来放崩溃，报错，当终点

int main() {

*p=20;//通过指针修改变量f的值为20,写在函数内
cout << "f=" << f << endl;

q=&f;//指针变量q存放的是变量f的地址，&取地址符，*取值符
*q=30;//通过指针修改变量f的值为30,写在函数内
cout << "f=" << f << endl;


printf("%d %f %f %f %c %s %f\n", a, b, c, d, e, s,g);

cout << a<<" "<< b << " "<< c << " "<< d << " "<< e << " "<< s <<endl;

return 0;
}