#include <iostream>
#include <random>
#include <string>
using namespace std;
/*17mm弹丸兑换比例：
远程兑换：150 金币/100 发
非远程兑换：10 金币/10 发
42mm弹丸兑换比例：
远程兑换：150 金币/10 发
非远程兑换：10 金币/1 发*/
// 编写球类和弹丸类
struct DanWan_size {
  string size;
};
struct DanWan_prise {
  int prise;
};
class ball {
public:
  struct DanWan_size stu_size_min = {"17mm"};
  struct DanWan_size stu_size_max = {"42mm"};
};
class projectile : public ball {
public:
  struct DanWan_prise stu_prise_min = {150};
  struct DanWan_prise stu_prise_max = {150};
  projectile(int danwan) {
    // 0表示获得小弹丸，1表示获得大弹丸
    if (danwan == 1) {
      cout << "获得大弹丸" << ",大小为:" << stu_size_max.size
           << ",价格为:" << stu_prise_max.prise << endl;
    } else {
      cout << "获得小弹丸" << ",大小为:" << stu_size_min.size
           << ",价格为:" << stu_prise_min.prise << endl;
    }
  }
};
// 前置的一些函数
int get_random_num(int min, int max) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(min, max);
  int random_num = dis(gen);
  return random_num;
}
int send_num(int a, int b) { return a + b; } // 传值调用
int cut_num(int &a) {
  int b = a / 2;
  return b;
}
// 以下为main函数
int main() {
  system("chcp 65001");
a:
  int A = 1, day = 1, L = 0, Q = 0; // L为昨天的数据,Q用来记录前天的数据
  if (day <= 100) {
    while (A <= 100 && day <= 100) {
      cout << "第" << day << "天的状态:" << endl;
      int condition_num = get_random_num(0, 1);
      if (day == 1) {
        cout << "我要成为视觉高手" << endl;
        A = send_num(A, L);
        cout << "今天的经验值A为:" << A << endl;
        L = A; // 记录此时A的值，以便传入else中
      } else if (day == 2) {
        cout << "我要成为视觉高手" << endl;
        Q = A;
        A = send_num(A, L);
        cout << "今天的经验值A为:" << A << endl;
        L = A; // 记录此时A的值，以便传入else中
      } else {
        if (A == 0) {
          goto b;
        }
        if (condition_num == 0) {
          cout << "我要成为视觉高手" << endl;
          A = send_num(L, Q); // 昨天+前天
          cout << "今天的经验值A为:" << A << endl;
          int tmp = L; // 备份昨天数据
          Q = tmp;     // 将昨天数据传入前天
          L = A;       // 跟新昨天的数据
        } else {
          cout << "我要成为打瓦高手" << endl;
          A = cut_num(L);
          cout << "今天的经验值A为:" << A << endl;
          int tmp = L; // 备份昨天数据
          Q = tmp;     // 将昨天数据传入前天
          L = A;       // 跟新昨天的数据
        }
      }
      day++;
    }
    cout << "第" << day - 1 << "天:" << endl;
    cout << "YOU ARE WELCOME TO JOIN PIONEER!" << endl;
    int danwan = get_random_num(0, 1);
    projectile get_danwan(danwan);
  } else {
    cout << "已经第100天了,你还没有成功" << endl;
  b:
    cout << "少打点瓦，多学习，继续努力" << endl;
  }
  return 0;
}