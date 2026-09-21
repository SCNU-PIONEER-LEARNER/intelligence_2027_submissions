#include<iostream>
using namespace std;

int main() {
	for (int i = 1;i <= 10000;i++) {
		if (i % 13 == 0) { cout << i << " "; }

	}
	cout << endl;
	return 0;
}



    int score;
    cout << "Enter score: ";
    if (!(cin >> score)) return 0;

    int result;
    if (score > 60)
    {
        result = 1;   // 1 表示合格
    }
    else
    {
        result = 0;   // 0 表示不合格
    }


    switch (result)
    {
    case 1:
        cout << "Pass" << endl;
        break;
    case 0:
        cout << "Fail" << endl;
        break;
    default:
        cout << "Invalid input" << endl;
    }

    return 0;
}

2穿透原理
#include<iostream>
using namespace std;
int main()
{
	int score = 0;
		cin >> score;
		switch (score / 10) {
		case 1:
	    case 2:
		case 3:
		case 4:
		case 5:
			cout << "f" << endl;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			cout << "r" << endl;
			break;
		default:
			cout << "w" << endl;

			return 0;


		}
}





















#include<iostream>
#include<random>
int main() {

	using namespace std;
	random_device rd;
	mt19937 gen(rd());

	uniform_int_distribution<int>dist(0, 30);
	cout << dist(gen) << endl;

	return 0;
}



#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
int main()
{

	int totalE = 0;
	int En1 = 1;
	int En2 = 1;
	int day = 1;
	int E = 0;
	int lastE = 0;
	int todayE = 0;
	srand((unsigned)time(0));

	while (totalE <100) {


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
			if (lastE > 0) {
				totalE -= lastE / 2;
				En2 = En1;
				En1 = todayE;
			}
			break;
		}




		}cout << totalE << endl;
		day++;
	}

	cout << "恭喜你成为视觉高手" << endl;
	return 0;
}
