#include<iostream>
#include<cstdlib>
#include<ctime>
#include<windows.h>
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
        SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8 编码
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
