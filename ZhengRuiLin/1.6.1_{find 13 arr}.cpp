#include<iostream>
using namespace std;
int main(){
    int cnt = 0;
    for(int i =1; i<=10000 ; i++){
        if(i%13==0){
            cnt++;
        }
    }
    int arr[cnt];
    for(int i =1,j=0; i<=10000 ; i++){
        if(i%13==0){
            arr[j] = i;
            j++;
        }
    }
    return 0;
}