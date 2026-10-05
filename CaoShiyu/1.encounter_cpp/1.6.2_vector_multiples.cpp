#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> multiples;

    for (int i = 1; i <= 10000; i++)
    {
        if (i % 13 == 0)
        {
            multiples.push_back(i);
        }
    }

    int printed = 0;

    for (int j : multiples)
    {
        cout << j << " ";
        printed++;

        if (printed % 10 == 0)
        {
            cout << endl;
        }
    }

    cout << endl;
    cout << "total count: " << multiples.size() << endl;

    return 0;
}