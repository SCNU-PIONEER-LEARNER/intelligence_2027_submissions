#include <iostream>
#include <vector>

class number{
    private:
        int  num;
    public:
        number(int n){
            num = n;
        }
        void print(){
            std::cout<<num<<std::endl;
        }
    };
std::vector<number> n13;
int main(){
    for(int i=1;i<10000;i++){
        if(i%13==0){
            n13.push_back(i);
        }
    }
    for(int a=0;a<n13.size();a++){
        n13[a].print();
    }
    return 0;
}