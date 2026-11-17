// 1.6.X_{task_name}.cpp 格式命名
// 1. 使?数组存 1?10000 中 13 的倍数
                    // #include <iostream>
                    // using namespace std;
                    // int main()
                    // {
                    //     int array[1000];
                    //     int count=0;
                    //     for (int i=1;i<=10000;i++)
                    //     {
                    //         if(i%13==0)
                    //         {
                    //             array[count]=i;
                    //             count++;
                    //         }
                    //     }
                    //     for(int i=0;i<count;i++)
                    //     {
                    //         cout<<array[i]<<" ";
                    //     }
                    //     return  0;
                    // }
// 2. 使? vector 存 1?10000 中 13 的倍数
                    // #include <iostream>
                    // #include <vector>
                    // using namespace std;
                    // int main()
                    // {
                    //     vector <int> v;
                    //         for (int i=1;i<=10000;i++)
                    //         {
                    //             if(i%13==0)
                    //             {
                    //                 v.push_back(i);
                    //             }
                    //         }
                    //         for(auto i:v)
                    //         {
                    //             cout<<i<<" ";
                    //         }
                    //     return  0;
                    // }
// 3. 使? vector 存 1?100，然后删除所有单数
                    // #include <iostream>
                    // #include <vector>
                    // using namespace std;
                    // int main()
                    // {
                    //     vector <int> v(100);
                    //         for (int i=1;i<=100;i++)
                    //         {
                    //             v[i-1]=i;
                    //         }
                    //         for(int i=v.size()-1;i>=0;i--)
                    //         {
                    //             if(v[i]%2==1)
                    //             {
                    //                 v.erase(v.begin()+i);
                    //             }
                    //         }
                    //         for(int i=0;i<v.size();i++)
                    //         {
                    //             cout<<v[i]<<" ";
                    //         }
                    //     return  0;
                    // }
// 4. 使? vector 存放任意数量的“弹丸”类，并尝试输出弹丸的类型（42mm or 17mm）
                    // #include <iostream>
                    // #include <ctime>
                    // #include <vector>
                    // #include <string>
                    // #include <cstdlib>
                    // using namespace std;
                    // class Ball
                    // {
                    // private:
                    //     double size;   
                    //     double price;  
                    // public:
                    //     Ball(double s, double p) : size(s), price(p) {}
                    //     double getsize() const
                    //     {
                    //         return size;
                    //     }
                    //     double getprice() const
                    //     {
                    //         return price;
                    //     }
                    //     virtual void showInfo() 
                    //     {
                    //         cout << "-----弹丸信息-----" << endl;
                    //         cout << "尺寸：" << getsize() << " mm" << endl;
                    //         cout << "价格：" << getprice() << " 元" << endl;
                    //     }
                    // };
                    // class BulletBall : public Ball
                    // {
                    // private:
                    //     string type;
                    // public:
                    //     BulletBall(double s, double p, string name) : Ball(s, p), type(name) {}
                    //     string gettype() 
                    //     {
                    //         return type;
                    //     }
                    //     void showInfo() 
                    //     {
                    //         cout << "-----弹丸信息-----" << endl;
                    //         cout << "弹丸种类：" << gettype() << endl;
                    //         cout << "尺寸：" << getsize() << " mm" << endl;
                    //         cout << "价格：" << getprice() << " 元" << endl;
                    //     }
                    // };
                    // int main()
                    // {
                    //     vector<Ball*> tool(10);
                    //     srand((unsigned)time(0));
                    //     int count=0;
                    //     for(int i=0;i<10;i++)
                    //     {
                    //         int x=rand()%2;
                    //         if(x==0)
                    //         {
                    //             tool[count]=new BulletBall(42.5, 10.0, "大弹丸");
                    //         }else{
                                
                    //             tool[count]=new BulletBall(16.8, 1.0, "小弹丸");
                    //         }
                    //         count++;
                    //     }
                    //     for(int i=0;i<10;i++)
                    //     {
                    //         tool[i]->showInfo();
                    //     }
                    //     return 0;
                    // }
// 5. 使? vector 分别存储 1 到 100 之间的单数、1 到 100 之间的双数、1 到 100 之间的质数的
// vector ，并输出同时是单数和质数的数值
                    // #include <iostream>
                    // #include <vector>
                    // using namespace std;
                    // bool judge(int n)
                    // {
                    //     if(n == 1) return false;
                    //     for(int i = 2; i*i <= n; i++)
                    //     {
                    //         if(n % i == 0)
                    //             return false;
                    //     }
                    //     return true;
                    // }
                    // int main()
                    // {
                    //     vector<int> jishu;   
                    //     vector<int> oushu;   
                    //     vector<int> zhishu;  
                    //     for(int i = 1; i <= 100; i++)
                    //     {
                    //         if(i % 2 == 1)
                    //         {
                    //             jishu.push_back(i);
                    //         }
                    //         else
                    //         {
                    //             oushu.push_back(i);
                    //         }

                    //         if(judge(i))
                    //         {
                    //             zhishu.push_back(i);
                    //         }
                    //     }
                    //     cout << "同时是单数和质数的数字：" << endl;
                    //     for(auto num : jishu)
                    //     {
                    //         if(judge(num))
                    //         {
                    //             cout << num << " ";
                    //         }
                    //     }
                    //     return 0;
                    // }
// 思考 ?： vector 和数组（ array ）有什么区别，任务?中是否引?过数组，能否使?
// vector 来优化代码；实现代码的?式有多种，但我们应选择最优美和简洁的那条。
                    // vector（容器）可以自动扩容，实用性来说比数组好，但就是因为自动扩容这个特点，导致下标容易越界，数组好就好在下标没那么容易越界，虽然使用过程中也会
                    // 越界，但是那是我们自己的问题（代码没有写好），所以我们可以根据情况来选择这两个东西


