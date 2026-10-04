#include <cstdio>      
#include <vector>      
class Bullet
{
public:                  
    int size;            
    Bullet(int s)
    {
        size = s;      
    }
};

int main()
{
    std::vector<Bullet> c;

    c.emplace_back(17);
    c.emplace_back(42);
    c.emplace_back(17);
 
    for (auto i : c)
    {
        printf("Bullet: %d mm\n", i.size); 
    }
    return 0; 
}