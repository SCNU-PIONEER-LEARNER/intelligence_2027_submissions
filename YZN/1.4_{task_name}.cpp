#include <cstdio>
#include <cstdlib>
int add_value(double a, double b)
{
	return a + b;
}
void add_ref(double a, double b, double& c)
{
	c = a - b;
}
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
			double a = add_value(a1, a2);
            A = A + a;
            a1 = a2;
            a2 = a;
        }
        else
        {
            double b = a2 / 2.0;
            add_ref(A, b, A);
        }
    }

    printf("YOU ARE WELCOME TO JOIN PIONEER!\n");
    return 0;
}