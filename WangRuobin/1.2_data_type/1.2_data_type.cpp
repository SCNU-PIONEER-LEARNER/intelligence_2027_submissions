#include <iostream>
#include<string> 
#include<iomanip>
int main(){
	int a=114514;
	float b=114.514;
	double c=3.141592653589793;
	char d ='a';
	std::string e="homo";
	std::cout<<"int是整数，例："<<a<<std::endl;
	std::cout<<"float是浮点数，例："<<b<<std::endl;
	std::cout<<std::setprecision(16);
	std::cout<<"double是双精度浮点数，例："<<c<<std::endl;
	std::cout<<"char是单个字符，需要用单引号，例：" <<d<<std::endl;
	std::cout<<"string是字符串，需要引用string头文件，用双引号，例："<<e<<std::endl;
	std::cout<< "nullptr是空指针，不太会用"<<std::endl; 
	return 0;
}
