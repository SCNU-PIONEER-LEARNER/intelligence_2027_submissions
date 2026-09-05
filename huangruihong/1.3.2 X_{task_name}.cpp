#include <iostream>
using namespace std;

int main() {
    int num;
    cout<<"请输入一个分数"<<endl;
    cin>>num;

    if(num<=100 && num>=0){
        switch(num/10){
            case 10:
            case 9:
            case 8:
            case 7:
            case 6:
                cout<<"及格"<<endl;
                break;
            case 5:
            case 4:
            case 3:
            case 2:
            case 1:
            case 0:
                cout<<"不及格"<<endl;
                break;
        }
    }
    else {
        cout<<"输入错误"<<endl;
    }
}

