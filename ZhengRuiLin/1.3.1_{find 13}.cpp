#include<iostream>
using namespace std;
int main(){
    for(int i = 1;i<=10000;i++){
        if(i%13==0){
            cout<<"1-10000中能被13整除的数有："<<i<<endl;
        }
    }
    return 0;
}