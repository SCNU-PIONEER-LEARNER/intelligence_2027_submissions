#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
    // ===== 防乱码：让 Windows 控制台按 UTF-8 显示中文 =====
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 用 sizeof 查看各数据类型占用的字节数
    std::cout << "int     占 " << sizeof(int) << " 字节" << std::endl;
    std::cout << "float   占 " << sizeof(float) << " 字节" << std::endl;
    std::cout << "double  占 " << sizeof(double) << " 字节" << std::endl;
    std::cout << "char    占 " << sizeof(char) << " 字节" << std::endl;
    std::cout << "string  占 " << sizeof(std::string) << " 字节（对象本身，与字符串长度无关）" << std::endl;

    // 定义并输出各种类型的变量
    int a = 42;
    float b = 3.14f;          // float 字面量要加 f 后缀
    double c = 3.14159265358979;
    char d = 'P';
    std::string e = "PIONEER";

    std::cout << "int a = " << a << std::endl;
    std::cout << "float b = " << b << std::endl;
    std::cout << "double c = " << c << std::endl;
    std::cout << "char d = " << d << std::endl;
    std::cout << "string e = " << e << std::endl;

    // nullptr：C++11 引入的空指针字面量，表示“指针不指向任何对象”
    int *p = nullptr;
    std::cout << "int* p 是空指针：" << (p == nullptr ? "是" : "否") << std::endl;

    // auto：由编译器根据初始值自动推导类型（它不是新数据类型，而是类型推导语法）
    auto x = 10;      // 推导为 int
    auto y = 3.14;    // 推导为 double
    std::cout << "auto x = " << x << "（推导为 int），auto y = " << y << "（推导为 double）" << std::endl;

    // ===== 思考题简答（详细版见 Word 文档）=====
    // 1. 什么是字节？ 字节(Byte)是计算机存储信息的基本单位，1 字节 = 8 位(bit)。
    // 2. 不同类型数据的大小相同吗？ 不相同：char 通常 1 字节、int 通常 4 字节、
    //    double 通常 8 字节，string 是对象，大小固定且与内容长度无关。
    // 3. 计算机的“内存”是什么意思？ 内存(RAM)是程序运行时存放数据和指令的地方，
    //    断电后数据丢失，速度比硬盘快几个数量级。
    // 4. 运行中把程序本身删掉，程序会停止吗？ 一般不会：程序运行时已加载进内存，
    //    删除磁盘上的文件不影响已运行的进程（Windows 甚至会阻止删除正在运行的程序文件）。
    // 5. auto 是数据类型吗？ 不是，auto 是类型推导关键字，编译器根据初始化表达式推断类型。

    return 0;
}
