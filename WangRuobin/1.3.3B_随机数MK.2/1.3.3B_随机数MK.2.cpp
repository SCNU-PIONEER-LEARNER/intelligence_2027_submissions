#include <iostream>
#include <random>
#include <chrono>
int main(){
	while(true){
		int n=0;
		int min=0;
		int max=0;
		std::cout<<"请输入输出随机数的个数"<<std::endl;
		std::cin>>n;
		std::cout<<"请输入随机区间的最小值"<<std::endl;
		std::cin>>min;
		std::cout<<"请输入随机区间的最大值"<<std::endl;
		std::cin>>max;
		unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
		std::cout << "种子是：" << seed << std::endl;
		std::mt19937 eng(seed);
		std::uniform_int_distribution<> rng(min,max);
		for(int i=1;i<=n;i=i+1){
			std::cout<<rng(eng)<<std::endl;
		}
	}
	return 0;
}
