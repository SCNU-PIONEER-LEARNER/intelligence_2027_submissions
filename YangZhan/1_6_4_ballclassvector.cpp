#include <iostream>
using namespace std;
#include <vector>

//弹丸类
class prt{//projectile
    public:
    int d;//直径

    prt(int D):d(D){}
    void type(){cout<<"直径为"<<d<<"mm 的弹丸"<<endl;}
};



int main(){
//创造弹丸对象
prt prt_a(17);
prt prt_b(42);
prt prt_c(17);

//类
vector<prt> vct;

//将对象存储到vector中
vct.emplace_back(prt_a);
vct.emplace_back(prt_b);
vct.emplace_back(prt_c);

for(auto it = vct.begin();it !=vct.end();++it){
    it->type();//解引用迭代器，拿到对象，并调用函数
}

return 0;
}