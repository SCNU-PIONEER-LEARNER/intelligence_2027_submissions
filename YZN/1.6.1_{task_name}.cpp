
#include <cstdio>
#include <vector>

int main(){

    std::vector<int> v;
    int i = 1;
    while (i <= 10000)
    {
        if (i % 13 == 0)
        {
            v.push_back(i);
        }
        i++;
    }
    for (int num : v)
    {
        printf("%d ", num);
    }

    return 0;
}