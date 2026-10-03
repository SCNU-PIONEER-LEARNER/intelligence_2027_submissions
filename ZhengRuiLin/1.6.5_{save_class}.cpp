#include<iostream>
#include<vector>
using namespace std;
int isprime_judge(int n);
int main(){
    vector<int> sin;
    vector<int> dou;
    vector<int> isprime;
    for(int i=0;i<=100;i++){
        if(i%2==0){
            dou.push_back(i);
        }
        else{
            sin.push_back(i);
        }
        if(isprime_judge(i)){
            isprime.push_back(i);
        }
    }
    for(int i = 1;i<sin.size();i++){
        if(isprime_judge(sin[i])){
            cout<<sin[i]<<endl;
        }
    }
    /*
    for(int num:sin){
        bool is_right = false;
        for(int num1:isprime){
            if(num == num1){
                is_right = true;
                break;
            }
        }
        if(is_right){
            cout<<num<<endl;
        }
    }
    */
    return 0;
}
int isprime_judge(int n){
    if(n<=1){
        return 0;
    }
    else{
        for(int i=2;i<n;i++){
            if(n%i==0){
                return 0;
            }
        }
    }
    return 1;
}