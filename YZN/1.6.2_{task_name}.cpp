#include <cstdio>      
#include <vector>       

int main()
{
    std::vector<int> v; 
    int i = 1;
    while(i<=100)
    {
        v.push_back(i); 
        i++;
    }

    for (int i = v.size() - 1; i >= 0; i--)
    {
        if (v[i] % 2 == 1) 
        {
            v.erase(v.begin() + i); 
        }
    }

        for (int num : v)
    {
        printf("%d ", num);
    }

    return 0;
}