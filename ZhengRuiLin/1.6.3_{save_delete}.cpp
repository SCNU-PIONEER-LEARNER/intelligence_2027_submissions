#include<iostream>
#include<vector>
using namespace std;
int main(){
    vector<int> save;
    for(int i =1; i<=100 ; i++){
        save.push_back(i);
    }
    for(int i = 0; i < save.size(); i++){
        if(save[i]%2==0){
            save.erase(save.begin()+i);
            i--;
        }
    }
    return 0;
}