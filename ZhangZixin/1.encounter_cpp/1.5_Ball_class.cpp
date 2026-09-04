#include <iostream>
#include <string>
#include <random>
#ifdef _WIN32
#include <windows.h>
#endif

// ================= 球（基类） =================
class Ball
{
protected:              // 保护成员：派生类可以访问，类外部不能直接访问
    std::string name_;  // 名称
    double radius_;     // 半径（毫米）
    double weight_;     // 质量（克）

public:                 // 公有成员：外部可以访问
    // 构造函数（初始化成员）
    Ball(const std::string &name, double radius, double weight)
        : name_(name), radius_(radius), weight_(weight)
    {
    }

    virtual ~Ball() = default;   // 虚析构函数：支持通过基类指针正确销毁派生类对象

    // 虚函数：展示信息（可在派生类中重写，实现多态）
    virtual void show() const
    {
        std::cout << name_ << "，半径 " << radius_ << " mm，质量 " << weight_ << " g";
    }

    std::string getName() const { return name_; }
};

// ================= 弹丸（派生类，继承 Ball） =================
class Projectile : public Ball
{
private:                // 私有成员：只有本类能访问
    double price_;      // 价格（元）
    std::string caliber_;  // 口径：42mm（大弹丸）或 17mm（小弹丸）

public:
    // 构造函数：先调用基类构造函数，再初始化自己的成员
    Projectile(const std::string &name, double radius, double weight,
               double price, const std::string &caliber)
        : Ball(name, radius, weight), price_(price), caliber_(caliber)
    {
    }

    // 重写基类的 show()（多态）
    void show() const override
    {
        Ball::show();
        std::cout << "，口径 " << caliber_ << "，价格 " << price_ << " 元";
    }

    std::string getCaliber() const { return caliber_; }
    double getPrice() const { return price_; }
};

// ================= 任务一：斐波那契小游戏（返回最终经验 A 和天数） =================
double runFibonacciGame(int &days)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    double A = 0.0;
    double last_E = 0.0;
    double e1 = 0.0, e2 = 0.0;
    int state0_count = 0;
    days = 0;

    while (A < 100)
    {
        ++days;
        int state = dist(gen);
        if (state == 0)   // 视觉高手：获得斐波那契经验
        {
            ++state0_count;
            last_E = (state0_count <= 2) ? 1.0 : (e1 + e2);
            e1 = e2;
            e2 = last_E;
            A += last_E;
        }
        else              // 打瓦高手：扣经验
        {
            A -= last_E / 2.0;
            if (A < 0)
                A = 0;
        }
    }
    return A;
}

int main()
{
    // ===== 防乱码 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 实例化大弹丸、小弹丸（尺寸和价格请自行观看比赛后了解，此处为合理默认值，可修改）
    Projectile big("大弹丸", 21.0, 41.0, 10.0, "42mm");
    Projectile small("小弹丸", 8.5, 3.2, 2.0, "17mm");

    std::cout << "比赛中的两种弹丸：" << std::endl;
    big.show();
    std::cout << std::endl;
    small.show();
    std::cout << std::endl << std::endl;

    // 与任务一结合：学习经验 A 达到 100 后随机奖励一个大弹丸或小弹丸
    int days = 0;
    double A = runFibonacciGame(days);
    std::cout << "经过 " << days << " 天，学习经验 A = " << A << "，成功加入 PIONEER 实验室！" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> coin(0, 1);

    if (coin(gen) == 0)
    {
        std::cout << "随机奖励：" << big.getName() << "（口径 " << big.getCaliber()
                  << "，价格 " << big.getPrice() << " 元）" << std::endl;
    }
    else
    {
        std::cout << "随机奖励：" << small.getName() << "（口径 " << small.getCaliber()
                  << "，价格 " << small.getPrice() << " 元）" << std::endl;
    }

    // 多态演示：基类指针指向派生类对象，调用虚函数 show() 会执行派生类版本
    std::cout << std::endl << "基类指针指向派生类（多态演示）：" << std::endl;
    Ball *p = &big;
    p->show();
    std::cout << std::endl;

    return 0;
}
