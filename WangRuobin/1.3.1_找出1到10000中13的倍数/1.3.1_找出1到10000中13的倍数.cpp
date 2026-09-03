#include <iostream>
int main(){
	for (int i=1;i<=10000;i=i+1){
		if(i%13==0){
			int n=i/13;
			std::cout<<i<<"=13x"<<n<<std::endl;
		}
	}
	return 0;
}
