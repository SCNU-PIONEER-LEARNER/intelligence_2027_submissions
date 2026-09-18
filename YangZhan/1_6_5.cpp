#include <iostream>
#include <vector>
using namespace std;
#include <algorithm>  //提供 find() 函数
int main(){
    vector<int> singular;//单数
    vector<int> even;//双数
    vector<int> prime;//质数

    //双数
    for(int i=1;i<=50;i++){
        even.emplace_back(i*2);
    }

    //单数
    for(int i=0;i<=49;i++){
        singular.emplace_back(i*2+1);
    }

    //质数

    //储存2~100
    for(int i=2;i<=100;i++){
        prime.emplace_back(i);
    }
    //删去合数
    for(int x=2;x<=10;x++){
        for (int y=2;y<=50;y++){
            int z=x*y;
            if(z>100){break;}//大于100则退出循环
            auto it=find(prime.begin(),prime.end(),z);//会一个个排查。若没有则返回end()
            if(it!=prime.end()){
                prime.erase(it);
            }
        }
    }



    //输出单数
    cout<<"单数:  ";
    for(int i:singular){
        cout<<i<<" ";
    }
    cout<<endl;

    //输出复数
    cout<<"复数:  ";
    for(int i:even){
        cout<<i<<" ";
    }
    cout<<endl;

    //输出质数
    cout<<"质数:  ";
    for(int i:prime){
        cout<<i<<" ";
    }
    cout<<endl;

    //输出单数且质数
    cout<<"输出单数且质数:  ";
    for(int i:singular){
        auto it=find(prime.begin(),prime.end(),i);//会一个个排查。若没有则返回end()
            if(it!=prime.end()){
                cout<<i<<" ";
            }
    }

return 0;
}
