#include <cstdio>//c++头文件
#include <iostream>//c++头文件,含cout,cin等输入输出流对象

using namespace std;//使用标准命名空间,可以直接使用cout等对象而不需要加std::前缀

int main()//main主函数,代码首先从main函数开始执行。int：整数，main执行完毕后会向操作系统返回一个整数结果
{
    printf("pioneer\n");//printf函数用于格式化输出，\n表示换行

    cout << "pioneer" ;
    return 0;//return 0表示程序正常结束，返回0给操作系统
}
//cout 与printf的区别：

//一，iostream 是type safe的，想输出莫名其妙的东西会直接报错。
//printf想输出莫名其妙的东西会输出乱码

// 二，其中cout中的存在suffer(缓存区),需要<<endl（flush+换行）
// 或<<flush来刷新，让缓存区的数据输出。也可以return 0强制刷新
//printf是C语言的输出函数，printf是直接输出到屏幕上，在windows中不存在缓存区。