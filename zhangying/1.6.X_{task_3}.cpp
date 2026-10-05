#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector<int>v;
    for(int i=1; i<=100; i++)
    {
        v.push_back(i);
    }
    for(vector<int>:: iterator it=v.begin(); it!=v.end(); )
    {
           if (*it%2==1)
           {
            it=v.erase(it);
           }
           else 
           {
            it++;
           }
    }
           for(vector<int>:: iterator it=v.begin(); it!=v.end();it++)
           {
            cout<< *it <<" "<<endl;
           }
           return 0;
}

