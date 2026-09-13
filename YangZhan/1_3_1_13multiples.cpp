//找到1——10000内13的倍数
#include <iostream>
using namespace std;
#include <cstdio>

int main(){
    for (int i=1;i<=10000;i++){
        if (i%13==0){
            cout<<i<<"  ";}
        
    }
    return 0;
}