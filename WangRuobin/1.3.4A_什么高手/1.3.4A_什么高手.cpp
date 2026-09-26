#include <iostream>
#include <random>
#include <chrono> 
#include <string>
int main(){
	double score=1.0;
	double dt1=1.0;
	double dt2=1.0;
	std::string cdt;
	
	for(;score<=100;){
		unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
		std::mt19937 eng(seed);
		std::uniform_int_distribution<> rng(1,10);
		int x=rng(eng)%2;
		switch(x+2){
			case 3:
			score=dt1+dt2;
			cdt="我要成为视觉高手！";
			std::cout<<x<<"今天的状态："<<cdt<<"----"<<"今日分数"<<score<<std::endl;
			break;
			case 2:
			score=dt1/2;
			cdt="我要成为打雷高手！";
			std::cout<<x<<"今天的状态："<<cdt<<"----"<<"今日分数"<<score<<std::endl;
			break;
		}
		dt2=dt1;
		dt1=score;
	}
	std::cout<<"YOU ARE WELCOME TO JOIN PIONEER!"<<std::endl;
	return 0;
}
