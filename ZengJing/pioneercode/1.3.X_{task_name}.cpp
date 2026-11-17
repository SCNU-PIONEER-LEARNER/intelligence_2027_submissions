// 1. 找出 1?10000 中 13 的倍数
                    // #include <iostream>
                    // #include <vector>
                    // using namespace std;
                    // int main()
                    // {
                    //     vector<int> tool;
                    //     int count=0;
                    //     for(int i=1;i<=10000;i++)
                    //     {
                    //         if(i%13==0)
                    //         {
                    //             tool.push_back(i);
                    //             count++;
                    //         }
                    //     }
                    //     for (auto i:tool)
                    //     {
                    //         cout<<i<<" ";
                    //     }
                    //     return 0;
                    // }
// 2. 输??个分数，?于 60 输出“合格”，?于 60 输出“不合格”（使? switch ）
                    // #include <iostream>
                    // using namespace std;
                    // int main()
                    // {
                    //     int x;
                    //     cin>>x;
                    //     switch (x/10){
                    //         case 10:
                    //         case 9:
                    //         case 8:
                    //         case 7:
                    //         case 6:
                    //         cout<<"合格"<<endl;
                    //         break;
                    //         defalut:
                    //         cout<<"不合格"<<endl;//偷懒了 
                    //         break;
                    //     }
                    //}

// 3. 使? C++ 的 random 库输出 0?30 之间的?个随机数
                        // #include <iostream>
                        // #include <random>
                        // using namespace std;
                        // int main()
                        // {
                        //     random_device seed;
                        //     mt19937 generate(seed());
                        //     uniform_int_distribution distribution(0,30);
                        //     cout<<distribution(generate)<<endl;
                        //     return 0;
                        // }
