#include <iostream>
#include <random>
using namespace std;

int main() {
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0,30 );

    int num =dist (gen);

    std::cout << num<< " ";

    return 0;
}

