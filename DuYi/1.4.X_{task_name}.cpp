#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
int add(int a, int b) {
	return a + b;
}
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
			
			totalE= add(totalE, ( - lastE) / 2);
			if (totalE < 0) {
				totalE = 0;
			}
			
			break;
		}




		}cout << totalE << endl;
		day++;
	}

	cout << "恭喜你成为视觉高手" << endl;
	return 0;
}













#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
void add(int& a, int b) { a = a + b; }
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
			if (day == 1 || day == 2) { totalE++; }
			else {

				add(totalE, (-lastE) / 2);
				if (totalE <= 0) {
					totalE = 2;
				}
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