#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;
class bullet
{
    private:
    string type;
    public:
    bullet(string t):type(t){}
    void t()
    {
        cout << type << endl;
    }
};
int main()
{
    SetConsoleOutputCP(65001);
    vector<bullet>v;
    v.push_back(bullet("大弹丸"));
    v.push_back(bullet("大弹丸"));
    v.push_back(bullet("小弹丸"));
    cout << "当前弹丸总数：" << v.size() << endl;
    for(vector<bullet>:: iterator it=v.begin(); it!=v.end();it++ )
    {
        it->t();
    }
}