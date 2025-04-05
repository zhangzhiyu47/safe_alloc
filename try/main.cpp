#define _DEBUG // 定义则模式为调试,不定义则默认为发布模式
#include <iostream>
#include "safe_alloc"
using namespace std;
init_safe_alloc; // 必须在使用之前定义

class Test
{
public:
    Test()
    {
        cout << "构造函数" << endl;
    }
    ~Test()
    {
        cout << "析构函数" << endl;
    }
public:
    int mStatus;
};

int main()
{
    // 调试模式代码
    Test* pTest=salloc.alloc<Test>(1,FileAndLine);
    salloc.free(pTest);
                                                          
    // 发布模式代码
    // Test* pTest=salloc.alloc<Test>(1);
    // salloc.free(pTest);
}
