#include<iostream>
#include<string>
using namespace std;

int main()
{
    int a=23;
    float b=3.14f;
    double c=3.1415926;
    char d='a';
    string e="PIONEER";
    int* p=nullptr;

    cout << "int    occupies " << sizeof(a) << " bytes" << endl;
    cout << "float  occupies " << sizeof(b) << " bytes" << endl;
    cout << "double occupies " << sizeof(c) << " bytes" << endl;
    cout << "char   occupies " << sizeof(d) << " byte" << endl;
    cout << "string occupies " << sizeof(e) << " bytes" << endl;
    cout << "pointer occupies " << sizeof(p) << " bytes" << endl;

    return 0;
}