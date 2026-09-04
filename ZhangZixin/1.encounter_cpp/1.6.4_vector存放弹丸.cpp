#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

// 简单弹丸类：记录口径类型
class Projectile
{
private:
    std::string caliber_;   // 口径：42mm（大弹丸）或 17mm（小弹丸）

public:
    explicit Projectile(const std::string &caliber) : caliber_(caliber)
    {
    }

    std::string getCaliber() const { return caliber_; }
};

int main()
{
    // ===== 防乱码 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 用 vector 存放任意数量的弹丸
    std::vector<Projectile> bullets;
    bullets.push_back(Projectile("42mm"));
    bullets.push_back(Projectile("17mm"));
    bullets.push_back(Projectile("42mm"));
    bullets.push_back(Projectile("17mm"));
    bullets.push_back(Projectile("42mm"));

    std::cout << "弹丸总数：" << bullets.size() << std::endl;
    for (size_t i = 0; i < bullets.size(); ++i)
    {
        std::cout << "第 " << i + 1 << " 颗弹丸类型：" << bullets[i].getCaliber() << std::endl;
    }
    return 0;
}
