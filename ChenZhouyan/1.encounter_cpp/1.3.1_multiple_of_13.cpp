#include <iostream>

using namespace std;

int main()
{
    // 任务：找出 1~10000 中 13 的倍数
    // 思路：从 13 开始，每次 +13 遍历（比逐个判断 i % 13 == 0 更高效）

    int count = 0;
    for (int i = 13; i <= 10000; i += 13)
    {
        cout << i << " ";
        count++;
        // 每 10 个换一行，输出更整齐
        if (count % 10 == 0)
            cout << endl;
    }

    cout << endl << "共 " << count << " 个 13 的倍数" << endl;

    return 0;
}
