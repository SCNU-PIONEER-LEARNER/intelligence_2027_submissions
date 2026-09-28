#include <iostream>

int main(){

    std::cout << "Multiples of 13 from 1 to 10000:\n";

    for (int number = 1; number <= 10000; ++number){

        if (number % 13 == 0){

            std::cout << number << ' ';
        }
    }

    std::cout << '\n';
    return 0;
}