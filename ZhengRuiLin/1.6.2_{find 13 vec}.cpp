#include<iostream>
#include<vector>
using namespace std;
int main(){
    vector<int> save;
    for(int i =1; i<=10000 ; i++){
        if(i%13==0){
            save.push_back(i);
        }
    }
    for(int i = 0; i < save.size(); i++){
        cout << save[i] << endl;
    }
    return 0;
}