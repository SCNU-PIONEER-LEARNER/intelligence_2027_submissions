#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int add_num(int a, int b)
{
    return a + b;
}



int main()
{
    srand(time(NULL));   // 初始化随机种子，只需要写一次

    int E = 1;
    int last = 0;

    int flag;

    while(E < 100 && E > 0)
    {
        // 每一天随机决定状态：0 或 1
        flag = rand() % 2;

        cout << "今天的状态是： " << flag << endl;

        if(flag == 0)    // 想成为视觉高手
        {
            int gain = add_num(E, last);
            E = add_num(E, gain);
            last = gain;

            cout << "今天选择：视觉高手" << endl;
            cout << "当前经验值为： " << E << endl;
        }
        else if(flag == 1)   // 想成为打瓦高手
        {
            E = add_num(E, -last / 2);

            cout << "今天选择：打瓦高手" << endl;
            cout << "当前经验值为： " << E << endl;
        }
    }
    cout << "当前经验值为： " << E
         << " 恭喜加入PIONEER战队！" << endl;
}
