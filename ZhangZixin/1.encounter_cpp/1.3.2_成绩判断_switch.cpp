#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
    // ===== 防乱码 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int score = 0;
    std::cout << "请输入分数：";
    std::cin >> score;

    // switch 只能判断整数，这里把分数除以 10 得到“十位档”：
    // 100→10，90~99→9，80~89→8，70~79→7，60~69→6，0~59→0~5
    // 因此 6~10 档判为“合格”，其余判为“不合格”（即 >=60 合格）
    switch (score / 10)
    {
    case 10:  // 100 分
    case 9:   // 90~99
    case 8:   // 80~89
    case 7:   // 70~79
    case 6:   // 60~69
        std::cout << "合格" << std::endl;
        break;
    default:  // 0~59 等
        std::cout << "不合格" << std::endl;
        break;
    }

    return 0;
}
