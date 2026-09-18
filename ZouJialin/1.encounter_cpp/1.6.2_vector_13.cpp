#include<iostream>
#include<vector>
using namespace std;

int main()
{
    vector<int> a;
    for(int i=1;i<10001;i++)
    {
        if(i%13==0)
        {
            a.push_back(i);
            cout<<i<<" ";
        }
    }
    return 0;
}