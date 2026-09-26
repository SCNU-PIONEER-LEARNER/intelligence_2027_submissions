#include <iostream>
#include <random>
#include <chrono>
#include <string>
class Ball{
    protected:
        std::string color;
        double price;
    public:
        Ball(std::string c,double p):color(c),price(p){}
        void show(){
            std::cout<<"颜色: "<<color<<std::endl;
            std::cout<<"价格: "<<price<<std::endl;
        }
        virtual ~Ball(){}
};
class shell:public Ball{
    private:
        std::string size;
    public:
        shell(std::string c,double p,std::string s):Ball(c,p),size(s){}
        void show(){
            std::cout<<"颜色: "<<color<<"价格"<<price<<"大小"<<size<<std::endl;
        }
};
double pcs0(double a){
    double b=a/2;
    return b;
}
double pcs1(double c,double d){
    double e=c+d;
    return e;
}
int Rdm(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 eng(seed);
    std::uniform_int_distribution<> distr(1, 10);
    int x=distr(eng)%2; 
    return x;
}
int main(){
    double dt1=1.0;
    double dt2=1.0;
    double score=1.0;
    int d=0;
    for (;score<=100.0;){
        int x=Rdm();
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
        shell small("red", 10.0, "small");
        shell big("blue", 20.0, "big");
        int y=Rdm();
        std::cout<<"你获得的奖励是："<<std::endl;
        if (y==1){ 
            big.show();
        }
        else{
            small.show();
         std::cout<<d<<"Days----"<<"you are welcome to join in PIONEER!"<< std::endl;
    return 0;
        }
}