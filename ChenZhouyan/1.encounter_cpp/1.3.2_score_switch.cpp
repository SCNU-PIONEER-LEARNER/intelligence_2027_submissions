#include <iostream>

using namespace std;

int main()
{
    // 任务：输入一个分数，大于 60 输出"合格"，小于 60 输出"不合格"（使用 switch）
    // 思路：switch 只能匹配整型常量，所以用 score / 10 把分数压缩成 0~10 的等级
    //       例如 75/10=7，命中 case 7；59/10=5，落到 default

    int score;
    cout << "请输入分数(0~100): ";
    cin >> score;

    // 输入合法性检查
    if (score < 0 || score > 100)
    {
        cout << "输入无效，分数必须在 0~100 之间" << endl;
        return 1;
    }

    switch (score / 10)
    {
    case 10:  // 100 分
    case 9:
    case 8:
    case 7:
    case 6:   // 60~69 分
        cout << "合格" << endl;
        break;
    default:  // 0~59 分
        cout << "不合格" << endl;
        break;
    }

    return 0;
}
