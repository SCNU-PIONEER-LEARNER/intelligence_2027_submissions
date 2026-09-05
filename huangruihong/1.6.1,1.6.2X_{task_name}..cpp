#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>




int main()
{
//使⽤数组arr存 1〜10000 中 13 的倍数
int arr[80];
int pos = 0;
for(int i=0;i<1000 ; i++)
{
    if(i%13==0)
    {
        arr[pos] = i;
        pos++;
    }
}   
for(int j=0;j<pos;j++)
{
    cout<<arr[j]<<" ";
}
    return 0;

//使⽤ vector 存 1~10000 中 13 的倍数
vector<int> v;

for(int n = 0; n<1000; n++)
{
    if(n%13==0)
    {
        v.push_back(n);
    }

}

for(int x:v)
{
    cout<<x<<" ";
}
    


}