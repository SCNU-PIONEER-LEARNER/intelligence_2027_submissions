#include <iostream>
using namespace std;

int main()
{
    const int maxNumber = 10000;
    const int divisor = 13;

    // 1~10000中最多有10000 / 13个13的倍数
    int multiples[maxNumber / divisor];
    int count = 0;

    // find out all multiples of 13 and save them in the array
    for (int i = 1; i <= maxNumber; i++)
    {
        if (i % divisor == 0)
        {
            multiples[count] = i;
            count++;
        }
    }

    // print all numbers in the array
    cout << "all multiples of 13 during 1 to 10000: " << endl;

    for (int i = 0; i < count; i++)
    {
        cout << multiples[i] << " ";

        // print a newline after every 10 numbers
        if ((i + 1) % 10 == 0)
        {
            cout << endl;
        }
    }

    cout << endl;
    cout << "total count: " << count << endl;

    return 0;
}