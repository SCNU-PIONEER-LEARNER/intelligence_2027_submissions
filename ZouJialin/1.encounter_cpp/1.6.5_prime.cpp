#include<iostream>
#include<vector>
using namespace std;
bool prime(int n)
{
    if(n<2)return false;
    for(int i=2;i<n;i++)
    {
        if(n%i==0)return false;
    }
    return true;
}
int main()
{
    vector<int> a,b,c;
    for(int i=1;i<=100;i++)
    {
        if(i%2==1) 
        {
            a.push_back(i);
            if(prime(i))
            {
                cout<<i<<" ";
            }
        }
        else b.push_back(i);
        if(prime(i))c.push_back(i);
    }
    return 0;
}