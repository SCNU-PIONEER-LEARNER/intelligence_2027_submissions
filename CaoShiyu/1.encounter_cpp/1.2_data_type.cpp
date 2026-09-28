#include<iostream>
#include<string>

int main()
{
    int date = 928;
    float height = 162.5f;
    double tele = 1.351126;
    char name = 'C';
    std::string team = "PIONEER";

    int* target_id = nullptr;
    auto task_index = 1126;

    std::cout << date << ' ' << height << ' ' << tele << ' ' << name << ' ' << team << std::endl;
    std::cout << "Task index: " << task_index << std::endl;

    return 0;
}