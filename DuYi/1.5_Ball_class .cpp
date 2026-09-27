#include <iostream>
#include <cmath>

class Ball {
private:
    // ===== 私有成员：外部不能直接改 =====
    double x, y, z;   // 球心坐标
    double radius;    // 半径

public:
    // ===== 构造函数 =====
    // 默认构造：球心在原点，半径 1
    Ball() : x(0), y(0), z(0), radius(1) {}

    // 带参构造：指定球心和半径
    Ball(double x, double y, double z, double r)
        : x(x), y(y), z(z) {
        radius = (r > 0) ? r : 1;   // 半径必须为正
    }

    // ===== 访问器（getter）=====
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    double getRadius() const { return radius; }

    // ===== 修改器（setter）=====
    void setCenter(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void setRadius(double r) {
        if (r > 0) radius = r;   // 非法值直接忽略
    }

    // ===== 功能函数 =====
    void move(double dx, double dy, double dz) {
        x += dx;
        y += dy;
        z += dz;
    }

    double surfaceArea() const {
        return 4 * 3.141592653589793 * radius * radius;
    }

    double volume() const {
        return (4.0 / 3.0) * 3.141592653589793 * radius * radius * radius;
    }

    bool intersects(const Ball& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        double dist = std::sqrt(dx*dx + dy*dy + dz*dz);
        return dist <= (radius + other.radius);
    }

    void print() const {
        std::cout << "球心: (" << x << ", " << y << ", " << z << ")"
                  << "  半径: " << radius << std::endl;
    }
};

int main() {
    Ball b1;                        // 默认构造
    Ball b2(3, 4, 0, 2);            // 带参构造

    b1.print();
    b2.print();

    b1.setRadius(5);                // 通过 setter 改半径
    b1.setCenter(0, 0, 0);
    std::cout << "修改后 ";
    b1.print();

    std::cout << "b1 体积: " << b1.volume() << std::endl;

    b1.move(1, 1, 1);
    std::cout << "移动后 ";
    b1.print();

    if (b1.intersects(b2))
        std::cout << "两球相交" << std::endl;
    else
        std::cout << "两球不相交" << std::endl;

    return 0;
}






#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
class Ball {
private:
	double x, y, z;
	double radius;

public:
	Ball() :x(0), y(0), z(0), radius(18) {}

	void setRadius(double r) {
		if (r > 0)
			radius = r;
	}

		void print() const {
			std::cout << "球心: (" << x << ", " << y << ", " << z << ")"
				<< "  半径: " << radius << std::endl;
		}

	};




	int add(int a, int b) {
		return a + b;
	}
	int main()
	{
		Ball b1;
		Ball b2;

		b2.setRadius(48);







		int totalE = 0;
		int En1 = 1;
		int En2 = 1;
		int day = 1;
		int E = 0;
		int lastE = 0;
		int todayE = 0;

		srand((unsigned)time(0));

		while (totalE < 100) {


			int result = rand() % 2;


			switch (result) {
			case 1: {


				cout << "我要成为视觉高手" << endl;
				if (day == 1 || day == 2) { todayE = En1; }
				else { todayE = En1 + En2; }
				En2 = En1;
				En1 = todayE;
				lastE = todayE;

				totalE += todayE;
				break;
			}
			case 0: {

				cout << "我要成为打瓦高手" << endl;

				totalE = add(totalE, (-lastE) / 2);
				if (totalE < 0) {
					totalE = 0;
				}

				break;
			}




			}cout << totalE << endl;
			day++;
		}

		cout << "恭喜你成为视觉高手" << endl;

		int encourage = rand() % 2;
		switch (encourage) {
		case 1: {
			cout << "获得奖励大弹丸价格十五金币" << endl;
			b2.print();

			break;

		}

		case 0: {
			cout << "获得奖励小弹丸价格一金币" << endl;
			b1.print();
			break;
		}
		}









		return 0;
	}









