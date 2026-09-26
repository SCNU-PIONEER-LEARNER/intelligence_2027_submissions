#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <string>
class shell{
    private:
    std::string size;
    public:
    shell(std::string s):size(s){}
    void show(){
        std::cout<<"弹丸的口径："<<size<<std::endl;
    }
};
int Rdm(std::mt19937 x){
    std::uniform_int_distribution<>distb(1,40);
    return distb(x);
}
std::vector<shell> sh;
int main(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 eng(seed);
    for (int i=0;i<40;i++){
        int rd=Rdm(eng)%2;
        switch(rd){
            case 0:
            sh.push_back(shell("17mm"));
            case 1:
            sh.push_back(shell("47mm"));
        }
    }
    int rd2=Rdm(eng);
    sh[rd2].show();
    return 0;
}