//用array储存1~10000中13的倍数

#include <iostream>
using namespace std;
#include <array>

int main(){
    array<int,769> mul;

    //储存
    int j=0;
    for (int i=1;i<=10000;i++){
        if(i%13==0){
            mul[j]=i;
            j++;
        }
    }

    //输出
    for(int i:mul){
        cout<<i<<" ";
    }
    return 0;
}