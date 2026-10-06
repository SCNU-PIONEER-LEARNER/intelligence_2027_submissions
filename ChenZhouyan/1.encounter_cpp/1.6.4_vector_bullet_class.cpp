// 1.6.4_vector_bullet_class.cpp —— vector 存任意数量的弹丸类，输出类型
#include <iostream>
#include <vector>
#include <random>
using namespace std;

// 复用任务五的弹丸类
class Bullet {
private:
    string name;
    double diameter;

public:
    Bullet(const string& n, double d) : name(n), diameter(d) {}
    string getName() const { return name; }
    double getDiameter() const { return diameter; }
};

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> coin(0, 1);

    // "任意数量"：数量也随机，10~30 发，随机装填大弹丸或小弹丸
    uniform_int_distribution<int> amount(10, 30);
    int n = amount(gen);

    vector<Bullet> magazine;  // vector 里直接放对象，容器管理内存，不用 new/delete
    for (int i = 0; i < n; i++) {
        if (coin(gen) == 0) magazine.push_back(Bullet("17mm 小弹丸", 17.0));
        else                magazine.push_back(Bullet("42mm 大弹丸", 42.0));
    }

    cout << "弹匣里装了 " << magazine.size() << " 发：" << endl;
    for (const Bullet& b : magazine) {   // 引用遍历，避免拷贝对象
        cout << "  - " << b.getName() << "（直径 " << b.getDiameter() << "mm）" << endl;
    }

    // 顺手统计两种弹丸各多少发
    int big = 0;
    for (const Bullet& b : magazine) {
        if (b.getDiameter() > 20) big++;
    }
    cout << "统计：42mm 大弹丸 " << big << " 发，17mm 小弹丸 "
         << magazine.size() - big << " 发" << endl;
    return 0;
}
