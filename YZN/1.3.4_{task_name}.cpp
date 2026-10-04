#include <cstdio>
#include <cstdlib>
int main()
{
    double a1 = 1.0;
    double a2 = 1.0;
    double A = 0.0;

    while (A < 100)
    {
       
        int s = rand() % 2;

        if (s == 1)
        {
            double a = a1+ a2;
            A = A + a;
            a1 = a2;
            a2 = a;
        }
        else
        {
            A = A - a2/2.0;
        }
    }

    printf("YOU ARE WELCOME TO JOIN PIONEER!\n");
    return 0;
}