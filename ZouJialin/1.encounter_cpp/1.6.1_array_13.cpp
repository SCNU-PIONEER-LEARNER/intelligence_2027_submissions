#include<iostream>
using namespace std;

int main()
{
    int arr[1000];
    int a=0;
    for(int i=1;i<10001;i++)
    {
        if(i%13==0)
        {   
            cout<<i<<" ";
            arr[a]=i;
            a++;
        }
    }
    return 0;
}