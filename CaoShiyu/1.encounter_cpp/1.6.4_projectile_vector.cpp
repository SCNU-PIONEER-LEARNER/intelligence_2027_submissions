#include <iostream>
#include <vector>
using namespace std;

class Projectile
{
private:
    int diameter;

public:
    Projectile(int inputDiameter)
    {
        diameter = inputDiameter;
    }

    void showType() const
    {
        cout << diameter << "mm projectile" << endl;
    }
};

int main()
{
    vector<Projectile> projectiles;
    int amount;

    cout << "How many projectiles do you want to store? ";
    cin >> amount;

    for (int i = 0; i < amount; i++)
    {
        int diameter;

        cout << "Enter projectile diameter (17 or 42): ";
        cin >> diameter;

        if (diameter != 17 && diameter != 42)
        {
            cout << "Only 17mm or 42mm is allowed." << endl;
            i--;
            continue;
        }

        projectiles.push_back(Projectile(diameter));
    }

    cout << "Stored projectiles:" << endl;

    for (const Projectile &projectile : projectiles)
    {
        projectile.showType();
    }

    return 0;
}