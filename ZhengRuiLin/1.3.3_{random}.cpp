#include<iostream>
#include<random>
using namespace std;
int get_random_number(int min,int max);
int main(){
    int num = get_random_number(0,30);
    cout<<"随机数是："<<num<<endl;
    return 0;
}
int get_random_number(int min,int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min,max);
    int num = dis(gen);
    return num;
}