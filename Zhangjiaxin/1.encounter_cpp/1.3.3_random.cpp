#include <iostream>
#include <random>
#include <windows.h>
using namespace std;

int main(){
    SetConsoleOutputCP(65001);

    // 使用 C++ 的 random 库生成 0~30 之间的一个随机数
    random_device rd;                                        // 随机数种子发生器
    mt19937 gen(rd());                                       // 梅森旋转引擎，用种子初始化
    uniform_int_distribution<int> dist(0, 30);               // 均匀整数分布：[0, 30]

    int num = dist(gen);

    cout << "0~30之间的随机数为：" << num << endl;

    return 0;
}
