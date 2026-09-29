#include <iostream>
using namespace std;

int main() {
    system("chcp 65001");
    int a=10;
    cout<<"a的字节数为:"<<sizeof(a)<<endl;
    float height = 1.75;
    double weight = 59.67;
    char score_rank = 'A';//用于字符与数字之间的转换
    char grade = score_rank + 1;
    string name ="PIONEER";
    int* ptr = nullptr;
    std::cout << "nullptr 赋值的空指针 ptr 的地址值：" << ptr << std::endl;
    std::cout << "nullptr 本身的类型是 decltype(nullptr)，占用字节数：" << sizeof(nullptr) << std::endl;
    cout<<"height的字节数为:"<<sizeof(height)<<endl;
    cout<<"weight的字节数为:"<<sizeof(weight)<<endl;        
    cout<<score_rank<<"score_rank的字节数为:"<<sizeof(score_rank)<<endl;
    cout<<grade<<"grade的字节数为:"<<sizeof(grade)<<endl;
    cout<<"name的字节数为:"<<sizeof(name)<<endl;


    return 0;
}