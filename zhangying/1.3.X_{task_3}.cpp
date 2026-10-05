#include <iostream>
#include <random>

using namespace std;

int main()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,30);

    int a=dist(gen);
    cout << a << endl;
    return 0;
}