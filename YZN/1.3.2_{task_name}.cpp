#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int main()
{
    int s;
    scanf("%d", &s);
        

    int t = s / 10;

    switch (t)
    {
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        printf("pass\n");
        break;
    default:
        printf("fail\n");
        break;
    }
    return 0;
}