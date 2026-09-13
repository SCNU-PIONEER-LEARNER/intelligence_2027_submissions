#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class Ball{

public:
    Ball(int D):m_D(D){}
    int m_D;
};

void printVector(Ball val){
    cout << val.m_D << endl;
}

int main(){

    vector<Ball> ball;

    Ball b1(17);
    Ball b2(42);
    Ball b3(17);
    Ball b4(42);
    Ball b5(17);

    ball.push_back(b1);
    ball.push_back(b2);
    ball.push_back(b3);
    ball.push_back(b4);
    ball.push_back(b5);

    for_each(ball.begin(),ball.end(),printVector);
    
    return 0;
}
