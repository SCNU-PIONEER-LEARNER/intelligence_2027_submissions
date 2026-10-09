#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector<int>v;
    for(int i=1; i<=10000 ;i++)
    {
        if(i%13==0)
        {
            v.push_back(i) ;
        }    
    }
    for (int j=0; j<v.size(); j++)
    {
        cout << v[j] <<" ";
    }
    return 0;
}