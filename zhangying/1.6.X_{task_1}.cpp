#include <iostream>
using namespace std;
int main()
{
    int arr[10000];
    int count=0;
    for(int i=1; i<=10000 ;i++)
    {
        if(i%13==0)
        {
            arr[count++]=i ;
        } 
    }
        for(int j=0; j<count; j++)
        cout<< arr[j] <<" ";
    
    return 0;
}