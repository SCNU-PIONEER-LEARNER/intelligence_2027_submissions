#include <iostream>
#include <random>
#include <chrono>
double pcs0(double a){
    double b=a/2;
    return b;
}
double pcs1(double c,double d){
    double e=c+d;
    return e;
}
int main(){
    double dt1=1.0;
    double dt2=1.0;
    double score=1.0;
    int d=0;
    for (;score<=100.0;){
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 eng(seed);
        std::uniform_int_distribution<> distr(1, 10);
        int x=distr(eng)%2; 
            if (x==1){
                score=pcs1(dt1,dt2);
                std::cout<<"今日状态是：我要成为视觉高手"<<"今日分数: " << score << std::endl;
            }
            else{
                score=pcs0(dt1);
                std::cout<<"今日状态是：我要成为打粥高手"<<"今日分数: " << score << std::endl;
            }
            dt2=dt1;
            dt1=score;
            d=d+1;
        }
         std::cout<<d<<"Days----"<<"you are welcome to join in PIONEER!"<< std::endl;
    return 0;
    }
    