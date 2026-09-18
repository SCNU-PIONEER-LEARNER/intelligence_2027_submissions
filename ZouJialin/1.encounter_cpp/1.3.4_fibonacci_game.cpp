#include<iostream>
#include<random>
#include<chrono>
using namespace std;

int main()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<int> state_dist(0, 1);

    int A = 0;               
    int last_learning = 0;  
    int a = 1, b = 1;        

    while (A < 100)
    {
        int state = state_dist(gen);

        if (state == 0) 
        {
            int gain = a;     
            int next = a + b; 
            a = b;
            b = next;

            A += gain;
            last_learning = gain;
        }
        else            
        {
            A -= last_learning / 2;
        }
    }
    cout << "YOU ARE WELCOME TO JOIN PIONEER!" << endl;
    return 0;
}
