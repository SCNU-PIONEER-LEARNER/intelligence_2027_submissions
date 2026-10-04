#include <cstdio>
#include <vector>
int xia(int n)
{
    if (n <= 1)
    {
        return 0;
    }
    for (int i = 2; i < n; i++)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    std::vector<int> a;    
    std::vector<int> b;   
    std::vector<int> c;  

    for (int i = 1; i <= 100; i++)
    {
        if (i % 2 == 1)
        {
            a.push_back(i);
        }
        else
        {
            b.push_back(i);
        }

        if (xia(i) == 1)
        {
            c.push_back(i);
        }
    }

    for (int i = 0; i < a.size(); i++)
    {
        printf("%d ", a[i]);
    }

    for (int i = 0; i < b.size(); i++)
    {
        printf("%d ", b[i]);
    }

    for (int i = 0; i < c.size(); i++)
    {
        printf("%d ", c[i]);
    }

    printf("\nOdd and Prime:\n");
    for (int i = 0; i < c.size(); i++)
    {
        int num = c[i];
        if (num % 2 == 1)
        {
            printf("%d ", num);
        }
    }

    return 0;
}