#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>

int main(){

vector<int> num;

//储存1-100
for(int i=1;i<=100;i++){
        num.push_back(i);
}

//用迭代器删除单数
// for (auto it=num.begin();it !=num.end();){
//     if (*it%2!=0){
//         it=num.erase(it);//erase返回下一个有效位置
//     }
//     else{it++;}
// }

//以上等效于erase-remove。
// //[]声明这是一个lambda函数（就地声明的匿名函数）。可以创建普通的函数bool isodd(int x) {return x % 2 != 0;}再将lambda换成isodd。
num.erase(remove_if(num.begin(), num.end(), [](int x) ->bool {return x % 2 != 0;}), num.end());

//输出vector
for (int i: num){
    cout<<i<<endl;
    }


return 0;
}



