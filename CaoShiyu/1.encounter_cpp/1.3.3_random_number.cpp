#include <iostream>
#include <random>

int main()
{
    std::random_device seed;
    std::mt19937 gener(seed());
    std::uniform_int_distribution<int> dis(0,30);

    int random_number = dis(gener);

    std::cout << "Random number: " << random_number << '\n';
    return 0;
}