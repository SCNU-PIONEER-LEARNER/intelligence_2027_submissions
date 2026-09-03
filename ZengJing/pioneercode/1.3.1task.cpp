// 任务?： 现在你应该对逻辑结构中的 while 、 if 、 switch 有了?定的认识，接下来我们做?
// 个“斐波那契数列”?游戏。
// 摘要：
// a. 你具有两种状态（每天的状态是随机的）：
// ?
// 我想成为视觉??，? 0 表?；
// ?
// 我想成为打???，? 1 表?；
// b. 当你处于状态（1）时，当天获得的学习经验为 E(n)；
// c. 奖励机制：每?天的结束都会根据前两天的状态进?奖励或惩罚。若你当天处于状态（1），那
// 么你将获得学习经验 E(n)=E(n-1)+E(n-2)；若你当天处于状态（2），那么你将减少学习经验
// L(n)=E(L)/2，E(L) 即为你上?次处于状态（1）时获得的学习经验；
// d. 当你的学习经验累积值 A 达到 100 时，你就成功加? PIONEER 机器?实验室，并在终端输
// 出 "YOU ARE WELCOME TO JOIN PIONEER!
#include <iostream>
#include <random>
using namespace std;
//暂且不考虑数字为负数的情况吧
double fscore(int x)
{
    if(x==0)
    {
        return 0;
    }else if(x==1 || x==2)
    {
        return 1;
    }
    int a=1,b=1,tool=0;
    for (int i=3;i<=x;i++)
    {
        tool=a+b;
        a=b;
        b=tool;
    }
    return tool;
}
int main()
{
    random_device seed;
    mt19937 generate(seed());
    uniform_int_distribution  distribution(0,1);
    double score=0.0;
    int learnday=0;
    while(1)
    {
        int random_number=distribution(generate);
        if(random_number==0)
        {
            learnday++;
            if(learnday==1 || learnday==2)
            {
                score+=1.0;
            }else{
                score+=(fscore(learnday-1)+fscore(learnday-2));
            }
            
        }
        if(random_number==1)
        {
            score=score-fscore(learnday)/2.0;
            learnday=0;
        }
        cout<<"现在的学习经验为"<<score<<endl;
        if(score>=100.0)
        {
            cout<<"YOU ARE WELCOME TO JOIN PIONEER!"<<endl;
            break;
        }
    }
    return 0;
}