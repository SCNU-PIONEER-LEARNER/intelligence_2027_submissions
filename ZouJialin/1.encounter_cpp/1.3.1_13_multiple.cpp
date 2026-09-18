#include<iostream>
using namespace std;
int main()
{
    for(int i=1;i<10001;i++)
    {
        if(i%13==0)
        {
            cout<<i<<" ";
        }
    }
    return 0;
}