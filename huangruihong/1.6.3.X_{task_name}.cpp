#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>


int main()
{
    
//使⽤ vector 存 1~100，然后删除所有单数
    vector<int> res;
    vector<int> positive;

    for(int i=1;i<=100;i++)
    {
        res.push_back(i);
    }

    for(int num : res)
    {
        if(num%2==0)
        {
            positive.push_back(num);
        }
    }

    cout<<"删除所有单数后，剩余的偶数为："<<endl;
    for(int even : positive)
    {
        cout<<even<<" ";
    } 
}
