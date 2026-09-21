#include <iostream>
using namespace std;
#include <random>
#include <algorithm>

double e1=1;
double e2=2;
double e3=0;
double l;

//传值调用
int PLUS(int a,int b){return a+b;};

int main(){

random_device rd;
mt19937 gen(rd());
bernoulli_distribution dist(0.5); //创建一个伯努利分布，概率为0.5



double A=0;//学习经验值


while (A<100){

auto state = dist(gen); //生成一个随机布尔值，true或false
    if (state) {//学习算法
        e3=PLUS(e1,e2);
        A+=e3;
        cout<<"学习加上"<<e3<<"  A为"<<A<<endl;
        e1=e2;
        e2=e3;
        e3=0;
    
    } 


    else {//打瓦
        l=e2/2;
        A=max(A-l,0.0);
        cout<<"打瓦扣除"<<l<<"  A为"<<A<<endl;
    }

}

cout<<"YOU ARE WELCOME TO JOIN PIONEER!";
return 0;
}