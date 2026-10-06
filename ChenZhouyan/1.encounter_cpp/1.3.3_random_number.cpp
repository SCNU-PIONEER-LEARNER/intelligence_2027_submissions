#include <iostream>
#include <random>
using namespace std;

// 用 C++ 的 <random> 库输出 0~30 之间的一个随机数
// 三步：random_device 拿种子 -> mt19937 引擎 -> uniform_int_distribution 限范围
// 比 rand()/srand() 那套好用，随机质量也更好

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 30);

    cout << "0~30 的随机数: " << dist(gen) << endl;

    return 0;
}
