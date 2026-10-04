#include <cstdio>

int main()
{
    int arr[800];       
    int c = 0;      
    int i = 1;           

    while (i <= 10000)   
    {
        if (i % 13 == 0)  
        {
            arr[c] = i; 
            c++;       
        }
        i++;               
    }

    for (int j = 0; j < c; j++) 
    {
        printf("%d ", arr[j]);
    }

    return 0;
}