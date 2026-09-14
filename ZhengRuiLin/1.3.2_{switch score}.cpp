#include<iostream>
using namespace std;
int main(){
    int score = 0;
    cin>>score;
    switch(score>=60){
        case 1:
            cout<<"及格"<<endl;
            break;
        case 0:
            cout<<"不及格"<<endl;
            break; 
    }
    return 0;
}