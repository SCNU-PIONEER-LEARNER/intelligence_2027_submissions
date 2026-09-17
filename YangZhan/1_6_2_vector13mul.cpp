#include <iostream>
using namespace std;
#include <vector>


int main(){

vector<int> num;

//储存
for(int i=1;i<=10000;i++){
    if (i%13==0){
        num.push_back(i);
    }
}

//输出
for (int i=0;i<=num.size()-1;i++){
cout<<num[i]<<endl;
}

return 0;
}