#include <iostream>
using namespace std;
#include <random>
#include <algorithm>

//球类
class ball{
    protected:
    string name;
    int size;
    int prise;

    public:
    //构造函数
    ball(string NAME,int SIZE,int PRISE):name(NAME),size(SIZE),prise(PRISE){}

};

class prt : public ball{//子类弹丸
    public:
    prt(string NAME,int SIZE,int PRISE):ball(NAME,SIZE,PRISE){
        cout<<"创建弹丸"<<name<<" 直径为"<<size<<"mm 价格为"<<prise<<"元"<<endl;
    }

    void get(){
        cout<<"得到了"<<name<<" 直径为"<<size<<"mm 价格为"<<prise<<"元"<<endl;
    }
};






double e1=1;
double e2=2;
double e3=0;
double l;

int main(){

//创建对象
prt prt_big("大弹丸",42,2);
prt prt_small("小弹丸",17,1);

random_device rd;
mt19937 gen(rd());
bernoulli_distribution dist(0.5); //创建一个伯努利分布，概率为0.5



double A=0;//学习经验值


while (A<100){

auto state = dist(gen); //生成一个随机布尔值，true或false
    if (state) {//学习算法
        e3=e1+e2;
        A=A+e3;
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

cout<<"YOU ARE WELCOME TO JOIN PIONEER!"<<endl;

if(dist(gen)){prt_big.get();}
else{prt_small.get();}

return 0;
}