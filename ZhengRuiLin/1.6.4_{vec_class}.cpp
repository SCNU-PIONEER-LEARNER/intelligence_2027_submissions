#include<iostream>
#include<vector>
#include<string>
using namespace std;
class bullet{
    private:
        string bullet_type;
        string bullet_size;
    public:
        bullet(string type,string size){
            bullet_type = type;
            bullet_size = size;
        }
        void show_bullet(){
            cout<<"子弹类型："<<bullet_type<<endl;
            cout<<"子弹大小："<<bullet_size<<endl;
        }
};
int main(){
    vector<bullet> bullets;
    bullet big_bullet("大弹丸","42mm");
    bullets.push_back(big_bullet);
    bullet small_bullet("小弹丸","17mm");
    bullets.push_back(small_bullet);
    for(int i=0;i<bullets.size();i++){
        bullets[i].show_bullet();
    }
    return 0;
}