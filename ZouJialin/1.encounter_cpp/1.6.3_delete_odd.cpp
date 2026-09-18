#include<iostream>
#include<vector>
using namespace std;

int main()
{
    vector<int> a;
    for(int i=1;i<101;i++)
    a.push_back(i);
    for(int i=a.size()-1;i>=0;i--)
    {
        if(a[i]%2==1)
        a.erase(a.begin()+i);
    }
    for(int i=0;i<a.size();i++)
    {
        cout<<a[i]<<" ";
    }
    return 0;
}