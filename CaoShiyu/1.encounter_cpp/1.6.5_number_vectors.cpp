#include <iostream>
#include <vector>
using namespace std;

bool isPrime(int number)
{
    if (number < 2)
    {
        return false;
    }

    for (int divisor = 2; divisor * divisor <= number; divisor++)
    {
        if (number % divisor == 0)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    vector<int> oddNumbers;
    vector<int> evenNumbers;
    vector<int> primeNumbers;

    for (int number = 1; number <= 100; number++)
    {
        if (number % 2 == 0)
        {
            evenNumbers.push_back(number);
        }
        else
        {
            oddNumbers.push_back(number);
        }

        if (isPrime(number))
        {
            primeNumbers.push_back(number);
        }
    }

    cout << "Numbers that are both odd and prime:" << endl;

    int count = 0;

    for (int number : primeNumbers)
    {
        if (number % 2 != 0)
        {
            cout << number << " ";
            count++;
        }
    }

    cout << endl;
    cout << "odd count: " << oddNumbers.size() << endl;
    cout << "even count: " << evenNumbers.size() << endl;
    cout << "prime count: " << primeNumbers.size() << endl;
    cout << "odd prime count: " << count << endl;

    return 0;
}