#include <cstdio>

int main()
{
    int i = 1;
    while (i <= 10000)
    {
        if (i % 13 == 0)
        {
            printf("%d ", i);
        }
        i = i + 1;
    }
    return 0;
}