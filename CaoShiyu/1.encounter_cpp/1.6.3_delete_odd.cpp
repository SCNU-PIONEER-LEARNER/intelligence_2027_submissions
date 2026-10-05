#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> numbers;

    for (int i = 1; i <= 100; i++)
    {
        numbers.push_back(i);
    }

    auto position = numbers.begin();

    while (position != numbers.end())
    {
        if (*position % 2 != 0)
        {
            position = numbers.erase(position);
        }
        else
        {
            position++;
        }
    }

    cout << "numbers after deleting odd numbers:" << endl;

    for (int j : numbers)
    {
        cout << j << " ";
    }

    cout << endl;
    cout << "total count: " << numbers.size() << endl;

    return 0;
}