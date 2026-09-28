#include <iostream>

int main()
{
    int score;

    std::cout << "Please enter a score from 0 to 100: ";
    std::cin >> score;

    if (score < 0 || score > 100)
    {
        std::cout << "Invalid score.\n";
        return 1;
    }

    switch (score / 10)
    {
        case 10:
        case 9:
        case 8:
        case 7:
        case 6:
            std::cout << "Pass\n";
            break;

        default:
            std::cout << "Fail\n";
            break;
    }
    return 0;
}