#include <iostream>
#include <random>
using namespace std;
int get_random_num(int min, int max) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(min, max);
  int random_number = dis(gen);
  return random_number;
}
int main() {
  system("chcp 65001");
  // 1.找出 1～10000 中 13 的倍数
  cout << "1~10000中13的倍数有:" << endl;
  for (int i = 1; i <= 10000; i++) {
    int dis_result = i % 13;
    if (dis_result == 0) {
      cout << i << endl;
    }
  }
  /*2.输入一个分数，大于 60 输出“合格”，小于 60 输出“不合格”（使用switch）  */
  int score;
  cout << "请输入一个分数" << endl;
  cin >> score;
  int rank;
  if (score >= 60) {
    rank = 1;
  } else {
    rank = 0;
  }
  switch (rank) {
  case 1:
    cout << "合格" << endl;
    break;
  default:
    cout << "不合格" << endl;
  }
  /*3.使用 C++ 的random库输出 0～30 之间的一个随机数*/
  int ran_num = get_random_num(1, 30);
  cout <<"生成一个1~30的随机数:"<< ran_num << endl;
  return 0;
}