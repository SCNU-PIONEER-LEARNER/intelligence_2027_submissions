#include <iostream>
#include <vector>
std::vector<int>n1;
std::vector<int>n2;
std::vector<int>n3;
std::vector<int>result;
int main(){
    int min=0;
    int max=0;
    while(true){
        std::cout<<"请输入最小值"<<std::endl;
        std::cin>>min;
        std::cout<<"请输入最大值"<<std::endl;
        std::cin>>max;
        bool qlf=true;
        if(min>0 && max>0){
            qlf=true;
        }
        else{
            qlf=false;
        }
        if(qlf==false){
            std::cout<<"请输入正整数"<<std::endl;
        }
        else{
            for (int a=min;a<max;a++){
                if(a%2==1){
                n1.push_back(a);
                }
            }
            for (int b=min;b<max;b++){
                if(b%2==0){
                n2.push_back(b);
                }
                    }
            for (int c=min;c<max;c++){
                int sc=0;
                for(int c2=1;c2<=c;c2++){
                    if(c%c2==0){
                        sc++;
                    }
                }
                if(sc==2){
                    n3.push_back(c);
                }
            }
            std::cout<<"数据分类已完成"<<std::endl;
            std::cout<<"同时为奇数和质数的是："<<std::endl;
            for(int x:n1){
                for(int y:n3){
                    if(x==y){
                        result.push_back(y);
                        break;
                    }
                }
            }
            for(int i=0;i<result.size();i++){
                std::cout<<result[i]<<std::endl;
            }
            n1.clear();
            n2.clear();
            n3.clear();
            result.clear();
        }          
    }       
}
    

