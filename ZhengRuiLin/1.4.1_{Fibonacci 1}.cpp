#include<iostream>
#include<random>
using namespace std;
int get_random_number(int min,int max);
int add(int a,int b);
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
                int e = add(e1, e2);
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
    return 0;
}
int get_random_number(int min,int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min,max);
    int num = dis(gen);
    return num;
}
int add(int a,int b){
    return a+b;
}