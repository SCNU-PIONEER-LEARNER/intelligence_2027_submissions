// 1.5_Ball_class.cpp —— 类与继承：球类 -> 弹丸类，接上任务一的经验小游戏
#include <iostream>
#include <string>
#include <random>
using namespace std;

// 任务1：球类（父类）
// private 里的东西外面摸不到，只能走 public 的接口，这就是访问限制的意义
class Ball {
private:
    string name;
    double diameter;  // 直径 mm
    int price;        // 单价（金币）

public:
    // 构造函数：创建对象时自动调用，把三个成员填好
    Ball(const string& n, double d, int p) : name(n), diameter(d), price(p) {}

    string getName() const { return name; }
    double getDiameter() const { return diameter; }
    int getPrice() const { return price; }

    void show() const {
        cout << name << "：直径 " << diameter << "mm，单价 " << price << " 金币" << endl;
    }
};

// 任务2：弹丸类，继承球（尺寸和价格按 RoboMaster 比赛里的设定来）
class Bullet : public Ball {
public:
    // 子类构造函数负责把参数转交给父类的构造函数
    Bullet(const string& n, double d, int p) : Ball(n, d, p) {}
};

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> state(0, 1);   // 每天的状态
    uniform_int_distribution<int> drop(0, 1);    // 奖励掉落用

    // 实例化两个弹丸：大弹丸和小弹丸（17mm 和 42mm，参考 RoboMaster）
    Bullet small("小弹丸", 17.0, 100);
    Bullet big("大弹丸", 42.0, 400);

    cout << "===== 仓库里的弹丸 =====" << endl;
    small.show();
    big.show();
    cout << endl;

    // 接任务一：随机状态攒经验，A 到 100 就"入伙"，然后随机发弹丸奖励
    int EpreV1 = 1, EpreV2 = 1;  // 最近两次处于状态1获得的经验
    int A = 0;                   // 经验积累值
    int day = 0;
    int lastState = -1;          // 记昨天状态，用来判断是否连续

    cout << "===== 开始打工攒经验 =====" << endl;
    while (A < 100 && day < 100) {
        day++;
        int s = state(gen);
        if (s == 1) {
            int E = (lastState == 1) ? EpreV1 + EpreV2 : 1;  // 连续打瓦才按斐波那契涨
            EpreV2 = EpreV1;
            EpreV1 = E;
            A += E;
            cout << "第" << day << "天 打瓦高手，经验 +" << E << "，A = " << A << endl;
        } else {
            int cost = EpreV1 / 2;  // 摸鱼扣上次经验的一半
            A -= (A >= cost) ? cost : A;
            cout << "第" << day << "天 视觉高手，经验 -" << cost << "，A = " << A << endl;
        }
        lastState = s;
    }

    cout << endl;
    cout << "YOU ARE WELCOME TO JOIN PIONEER!" << endl;

    // 随机奖励一枚弹丸
    const Bullet& prize = (drop(gen) == 0) ? small : big;
    cout << "入伙奖励：抽中了 " << prize.getName() << "！直径 "
         << prize.getDiameter() << "mm，价值 " << prize.getPrice()
         << " 金币，已放进你的储物柜。" << endl;
    return 0;
}
