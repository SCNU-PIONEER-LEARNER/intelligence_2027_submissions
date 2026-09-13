#include<iostream>
using namespace std;

int main(){

    int array[10000/13];
    int count = 0;
    for(int i = 1; i < 10000; i++){
        if(i % 13 == 0){
            array[count] = i;
            count++;
        }
    }
    cout << "数组：1~10000中13的倍数" << endl;
    for(int i = 0;i < sizeof(array)/sizeof(int);i++){
        cout << array[i] << endl;
    }
    return 0;
}