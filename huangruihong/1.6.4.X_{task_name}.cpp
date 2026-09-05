#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>
#include <string>

class Projectile
{
public:
    double size;
    string color;
    
    void show()
    {
        if(size==17)
        {
            cout<<"这是小弹丸"<<",颜色为"<<color<<endl;
        }
        
        else if(size==42)
        {
            cout<<"这是大弹丸"<<",颜色为"<<color<<endl;
        }
    }
};


int main()
{
    vector<Projectile>  vec;


    Projectile p1;
    p1.size = 17;
    p1.color = "蓝色";
    vec.push_back(p1);


    Projectile p2;
    p2.size = 42;
    p2.color = "红色";
    vec.push_back(p2);

    Projectile p3;
    p3.size = 17;   
    p3.color = "金色";
    vec.push_back(p3);

    for(auto &obj : vec)
    {
        obj.show();
    }
    return 0;
}


