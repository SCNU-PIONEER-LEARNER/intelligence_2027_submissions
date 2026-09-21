//使⽤C++的random 库输出0〜30之间的⼀个随机数

//random库：用于随机数生成。由随机数引擎和随机数分布两部分组成。
#include <iostream>
#include <random>
using namespace std;

//随机数引擎：用于生成随机数的算法。常⽤的随机数引擎：std::mt19937：梅森旋转算法（Mersenne Twister）等

//随机数分布：（disribution)
//均匀分布整数：std::uniform_int_distribution<>
//均匀分布浮点数：std::uniform_real_distribution<>
//正态分布：std::normal_distribution<>
//伯努利分布（只生成true和false）：std::bernoulli_distribution

int main(){

    //创建引擎
    random_device rd; //真随机数种子,用于启动引擎，起名为rd
    cout << "rd() = " << rd() << endl; //输出rd()的值
    mt19937 gen(rd());//使用rd()的值作为种子，创建一个随机数引擎起名为gen，伪随机数，节省性能

    //创建分布
    uniform_int_distribution<int> dist(0, 30); //创建一个均匀分布整数，范围是0~30，起名为dist
    
    //用dist(gen)生成随机数
    for (int i=0;i<=4;i++){
        cout <<"随机数"<<i+1<<"为"<<dist(gen)<<endl;
    }

    return 0;
}