#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

void printVector(int val){
    cout << val << endl;
}

int main(){

    vector<int> v;
    for(int i = 1;i <= 100;i++){
        v.push_back(i);
    }

    auto it = v.begin();
    while (it != v.end())
    {
        if (*it % 2 != 0)
        {
            it = v.erase(it); 
        }
        else
        {
            ++it;
        }
    }

    cout << "vector：存 1〜100，然后删除所有单数" << endl;
    for_each(v.begin(), v.end(), printVector);
    return 0;
    
}