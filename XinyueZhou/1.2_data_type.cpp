
#include <iostream>
#include<windows.h>
using namespace std;
int main() {
	SetConsoleOutputCP(CP_UTF8);
	cout << "什么是字节？" << endl;
	cout << "字节是计算机的最小储存单位。1字节=8个二进制组成（即0、1）" << endl;
	
	//int是整型函数类型（整数
	int age = 18;
	//float、double是实型函数类型（带小数
	float weight = 50.6;
	double height = 167.8;
	//char字符型
	char gender = 'F';
	//string字符串型
	string name = "小悦";
	//nullptr指针型
	nullptr_t np = nullptr;
	cout << "不同类型的数据的⼤⼩相同吗？\n不同。" << endl;
	cout << "int大小：" << sizeof(age) << "字节" << endl;
	cout << "float大小：" << sizeof(weight) << "字节" << endl;
	cout << "double大小：" << sizeof(height) << "字节" << endl;
	cout << "char大小：" << sizeof(gender) << "字节" << endl;
	cout << "string大小：" << sizeof(name) << "字节" << endl;
	cout << "nullptr大小：" << sizeof(np) << "字节" << endl;

	cout << "计算机的“内存”是什么意思？\n程序运行时临时存放数据的地方" << endl;
	cout << "如果程序正在运行中，把程序本身删掉，程序会停停止运行吗?\n不能。" << endl;
	cout << "auto 是数据类型吗？\n?" << endl;
	

	return 0;

}

