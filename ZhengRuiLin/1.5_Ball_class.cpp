#include<iostream>
#include<random>
#include<string>
using namespace std;
int get_random_number(int min,int max);
class Ball{
    protected:
        int radius;
        int money;
    public:
        Ball(int r,int m):radius(r),money(m){}
        void show(){
            cout<<"球的半径是："<<radius<<"球的价格是："<<money<<endl;
        }
};
class bullet:public Ball{
    private:
        string bullet_type;
    public:
        bullet(int r,int m,string b_t):Ball(r,m),bullet_type(b_t){}
        void show_bullet(){
            cout<<"子弹的类型是："<<bullet_type<<"子弹的半径是："<<radius<<"子弹的价格是："<<money<<endl;
        }

};
int main(){
    int e1 = 1;
    int e2 = 1;
    int e_l = 1;
    bool experience = true;
    int day = 0;
    int equal = 0;
    while(experience){
        int condition = get_random_number(0,1);
        switch(condition){
            case 0: {
                int e = e1 + e2;
                equal += e;
                e1 = e2;
                e2 = e;
                e_l = e;
                day++;
                cout<<"第"<<day<<"天，今日想成为视觉高手，获得经验是"<<e<<"剩余总经验是："<<equal<<endl;
                break;
            }

            case 1: {
                int l_n = e_l/2;
                equal -= l_n;
                if (equal < 0) {
                    equal = 0;
                }
                day++;
                cout<<"第"<<day<<"天，今日想成为打瓦高手，失去经验是"<<l_n<<"剩余总经验是："<<equal<<endl;
                break;
            }

        }
        if(equal >= 100){
            experience = false;
            cout<<"YOU ARE WELCOME TO JOIN PIONEER!"<<endl;
            break;
        }
    }
    int gift = get_random_number(0,1);
    if(equal >=100){
        if(gift == 0){
            bullet big_bullet(42,100,"大子弹");
            big_bullet.show_bullet();
        }
        else{
            bullet small_bullet(17,50,"小子弹");
            small_bullet.show_bullet();
        }
    }

}
int get_random_number(int min,int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min,max);
    int num = dis(gen);
    return num;
}