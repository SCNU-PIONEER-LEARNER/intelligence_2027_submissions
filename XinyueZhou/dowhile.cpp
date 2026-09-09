

#include <iostream>
#include "random"
using namespace std;
int get_random_num(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution <> dis(min, max);
    int random_number = dis(gen);
    return random_number;
}

int main()
{
    int num = get_random_num(1, 100);
    int guess_num;
    do {
        cout << "请输入一个猜测的数字：";
        cin >> guess_num;
        if (guess_num > num) {
            cout << "不好意思猜错了，你猜【大】了。" << endl;
        }
        else if(guess_num<num){
            cout<< "不好意思猜错了，你猜【小】了。" << endl;

        }
        else {
            cout << "恭喜你猜对了！" << endl;

        }
    } while (guess_num != num);
    return 0;

}

