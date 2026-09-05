#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>
#include <string>

int main()
{   

    vector<int> v1;
    for(int i=1;i<100;i++)
    {   
        if(i%2 !=0)
        v1.push_back(i);
    }

    vector<int> v2;
    for(int n=1;n<100;n++)
    {   
        if(n%2==0)
        v2.push_back(n);
    }

 vector<int> v3;
// 求2~100所有质数存入v3
for(int k = 2; k <= 100; k++)
{
    bool is_prime = true; //标记是不是质数
    for(int j = 2; j < k; j++) //拿2到k‑1去试除
    {
        if(k % j == 0)
        {
            is_prime = false;
            break; //不是质数，直接结束内层循环
        }
    }
    if(is_prime)
    {
        v3.push_back(k);
    }
}



    vector <int> res;
    res.push_back(2);
    for(auto &a :v1)
    {
        for (auto &b :v3)
        {
            if(a==b)
            {
                res.push_back(a);
            }
            
        }
           
    }    
    

    for(auto &x:res)
    {
        cout<<x<<" ";
    }
    return 0;




}