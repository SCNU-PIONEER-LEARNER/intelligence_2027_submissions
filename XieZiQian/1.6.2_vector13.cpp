#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

void printVector(int val){
    cout << val << endl;
}
int main(){

    vector<int> v;
    for(int i = 1; i < 10000; i++){
        if(i % 13 == 0){
            v.push_back(i);
        }
    }
    cout << "vector£º1~10000ÖÐ13µÄ±¶Êý" << endl;
    for_each(v.begin(), v.end(), printVector);

    return 0;
}