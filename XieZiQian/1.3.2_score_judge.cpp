#include<iostream>
using namespace std;

int main(){

    int num = 0;
    cin >> num;
    if(cin.fail())
    {
        cout << "输入错误" << endl;
        
    }else if(num < 0 || num > 100){
        cout << "请输入0~100之间的数" << endl;
    }else{
        switch(num/10)
        {
        case 10:
        case 9:
        case 8:
        case 7:
        case 6:
            cout << "合格" << endl;
            break;
        case 5:
        case 4:
        case 3:
        case 2:
        case 1:
        case 0:
            cout << "不合格" << endl;
            break;
        }
    }
    
    return 0;
}