#include <iostream>
int main(){
	while(true){
		int grade=0;
		bool j=false;
		std::cout<<"请输入成绩"<<std::endl;
		std::cin>>grade;
		if (grade<60){
			j=false;
			
		}
		else{
			j=true;
		}
		switch(j){
			case true:
				std::cout<<"合格，捞捞"<<std::endl; 
				break;
			
			case false:
				std::cout<<"不合格，挂挂" <<std::endl; 
				break;
			
		}
	}
	
	return 0;
}

