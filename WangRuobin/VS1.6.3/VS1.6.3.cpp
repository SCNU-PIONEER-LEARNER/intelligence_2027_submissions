#include <iostream>
#include <vector>
#include <algorithm>

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
        bool judge() const{
            return num % 2 != 0;
        }
    };
std::vector<number> no;
int main(){
    for (int i=1;i<100;i++){
        no.push_back(number(i));
    }
no.erase(std::remove_if(no.begin(), no.end(), [](const number& x){ return x.judge(); }), no.end());
    for(int a=0;a<no.size();a++){
        no[a].print();
    }
    return 0;
}