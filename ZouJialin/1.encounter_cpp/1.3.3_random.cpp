#include<iostream>
#include<random>
#include<chrono>
using namespace std;

int main()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(0, 30);

    cout<<dist(gen)<<endl;
    return 0;
}
