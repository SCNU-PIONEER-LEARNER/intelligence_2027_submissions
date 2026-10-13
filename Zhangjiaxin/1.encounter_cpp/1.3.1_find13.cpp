#include <iostream>
#include <windows.h>
using namespace std;

int main(){
    SetConsoleOutputCP(65001);
    int num = 1;
    
    cout <<"1~10000中13的倍数有："<<endl;
    while(num <= 10000){
        if(num % 13 == 0){
            cout << num <<endl;
        }
        num += 1;
    }
    return 0;
}