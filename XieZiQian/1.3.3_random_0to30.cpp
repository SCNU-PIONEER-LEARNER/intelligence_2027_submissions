#include<iostream>
#include <random>
using namespace std;

int main(){
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 30);

    int num = dist(gen);
    cout << "0~30之间的随机数：" << num << endl;
    return 0;
}