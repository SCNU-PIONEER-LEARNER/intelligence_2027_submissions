#include <iostream>
#include <vector>
using namespace std;
void myprint(vector<int> &v) {
  for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
    cout << *it << " ";
  }
  cout << endl;
}
class DanWan {
public:
  DanWan() {}
  DanWan(string ball1, string ball2) {
    minsize = ball1;
    maxsize = ball2;
  }
  void showInfo() { cout << "Min:" << minsize << ", Max:" << maxsize << endl; }
  ~DanWan() {}

private:
  string maxsize;
  string minsize;
};
int main() {
  system("chcp 65001");
  // 1.使用数组存 1～10000 中 13 的倍数
  int maxsize = 10000 / 13;
  int *p = new int[maxsize];
  int size_temp = 0;
  for (int i = 0; i <= 10000; i++) {
    if (i % 13 == 0) {
      p[size_temp] = i;
      size_temp++;
    }
  }
  for (int i = 0; i < size_temp; i++) {
    cout << p[i] << endl;
  }
  // 2.存入1~10000的数
  vector<int> v1;
  for (int i = 1; i <= 10000; i++) {
    if (i % 13 == 0) {
      v1.push_back(i);
    }
  }
  cout << "13的倍数有:" << endl;
  myprint(v1);
  // 3. 存入1~100，并删去单数
  vector<int> v2;
  for (int i = 1; i <= 100; i++) {
    if (i % 2 != 1) {
      v2.push_back(i);
    }
  }
  cout << "1~100中删去单数后:" << endl;
  myprint(v2);
  // 4.存放任意数量的“弹丸”类，并尝试输出弹丸的类型（42mm or 17mm）
  vector<DanWan> ball;
  ball.push_back(DanWan("17mm", "42mm"));
  for (int i = 0; i < ball.size(); i++) {
    ball[i].showInfo();
  }
  // 5.存储 1 到 100 之间的单数、
  vector<int> v3, v4, v5;
  for (int i = 1; i <= 100; i++) {
    if (i % 2 == 1) {
      v3.push_back(i);
    }
  }
  cout << "1~100的单数:" << endl;
  myprint(v3);
  // 5.存入1 到 100 之间的双数
  v4 = v2;
  cout << "1~100的双数:" << endl;
  myprint(v4);
  // 5.存入1 到 100 之间的质数
  v5.push_back(2);
  v5.push_back(3);
  for (int i = 5; i <= 100; i++) {
    if (i % 6 == 1 || i % 6 == 5) {
      v5.push_back(i);
    }
  }
  cout << "1~100的质数:" << endl;
  myprint(v5);
  // 在所有的质数中只有2是偶数，只需要删除就可以了
  v5.erase(v5.begin());
  cout << "1~100的单数且质数:" << endl;
  myprint(v5);
  return 0;
}