#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

void printSinglePrimeVector(int val){
    if(val % 2 != 0){
        cout << val << " ";
    }
}

void printVector(int val){
    cout << val << " ";
}

int main(){

    vector<int> v1;//单数
    vector<int> v2;//双数
    vector<int> v3;//质数

    
    for(int i = 1;i <= 100;i++){

        if(i % 2 != 0){
            v1.push_back(i);
        }else{
            v2.push_back(i);
        }

        //判断质数
        bool isPrime = true;
        if(i < 2) isPrime = false; // 1不是质数
        for(int j = 2 ;j < (int)(sqrt(i)+1);j++){
            if(i%j==0){
                isPrime = false;
                break;
            }
        }
        if(isPrime){
            v3.push_back(i);
        }
    }

    cout << "单数" << endl;
    for_each(v1.begin(),v1.end(),printVector);
    cout << endl;

    cout << "双数" << endl;
    for_each(v2.begin(),v2.end(),printVector);
    cout << endl;

    cout << "质数" << endl;
    for_each(v3.begin(),v3.end(),printVector);
    cout << endl;

    cout << "同时是单数和质数的数" << endl;
    for_each(v3.begin(),v3.end(),printSinglePrimeVector);
    return 0;
    
}