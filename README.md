# C++智能分配释放空间追踪类safe_alloc说明及其用法

## 1.说明
safe_alloc(后简称为该库)基于C++的**STL**和**GCC**开发的智能分配释放空间追踪类，被实现在std命名空间里。  
**safe_alloc**只有一个来自本身的对象，即为单例模式。已将构造、析构、拷贝构造和重载赋值全部私有化，无法继承。  
同时，若使用C语言风格的malloc等的分配方式申请的safe\_alloc大小的堆区空间或随便指向不合法的空间，并使用safe\_alloc*进行解引用，会抛出this\_no\_true异常，因为内部可以判断this是否为唯一的对象。  
制作人:**张志宇**  
(**虽源码公开，但不建议随意更改源码!**)

## 
## 2.用法
### 1.复制文件
把该文件所在的目录下的`source`中的`safe_alloc`文件复制到要用到该库的文件所在的目录下。
### 2.定义模式
默认的模式为**Release**发布模式。运行快，节约空间，但追踪信息没有Debug模式完整。  
在代码中插入`#define _DEBUG`进入**Debug**调试模式(***每个文件都要，否则模式功能无效！！！***G++可以加上`-D_DEBUG`参数自动为每一个文件添加\_DEBUG宏，Visual Studio的Debug32/64自动定义了\_DEBUG宏)。运行比Release慢，使用了更多的空间，但追踪信息完整。  
(**注意**:这两个模式都基于宏定义完成，即在预编译后模式已经定型)
### 3.引入头文件
在代码中插入`#include "safe_alloc"`(也可以直接把这个文件放在对应的GCC的include目录(一般是`/usr/include/`)，然后无论在哪里，都可以直接引入`#include <safe_alloc>`)  
(**注意**:若前面定义了Debug模式，则***一定要引入在***`#define _DEBUG`***的下面***，否则模式定义有可能失效)
### 4.初始化
***在头文件引入后插入全局***代码`init_safe_alloc;`来初始化safe\_alloc类，切记***必须且只能初始化一次，多次初始化就会出错(多文件也一样)！！！***,并定义safe\_alloc类唯一的对象std::salloc(可以'.'出的都是类内方法)。  
-----*不初始化就使用该类的方法大概会报错undefined symbol: std::salloc&std::safe\_alloc::sa(没有定义的变量(标识符):std::salloc和std::safe\_alloc::sa)。*  
-----*如果多次初始化大概会报错redefinition of 'std::safe\_alloc::sa'&'std:salloc'("std::safe\_alloc::sa"和"std:salloc"重复定义)。*
### 5.可以开始使用了
一定要仔细阅读该库的用法！

## 
## 3.介绍
**1.**`safe_alloc::space_info`**空间信息类**  
+size_t num;申请的数量  
+char * name;申请的变量名  
以下为Debug模式独有  
--char * file;申请代码所在的文件名  
--size_t size;申请变量的单个字节数  
--size_t line;申请代码所在的行数  

**2.**`FileAndLine`**宏定义**  
+定义:`#define FileAndLine`  
+作用:直接用于参数结尾为char* file,size_t line的函数中，直接FileAndLine一个参数即可  

**3.**`get_self`**类内方法**  
+声明:`safe_alloc& get_self(void);`  
+作用:获取单例模式的唯一对象  
+参数:无参  
+返回:safe_alloc&  

**4.**`information`**类内方法**  
+声明:`pair<bool,safe_alloc::space_info> information(void *space) const;`  
+作用:查找space空间的信息  
+参数:space要查找的空间  
+返回:pair<bool,safe\_alloc::space\_info>，bool为查找是否成功  
+异常:this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  

**5.**`number`**类内方法**  
+声明:`size_t number(void) const;`  
+作用:获取未释放的申请次数  
+返回:未释放的申请次数  
+异常:this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  

**6.**`is_debug`**类内方法**  
+声明:`bool is_debug(void) const;`  
+作用:判断是否为调试模式  
+参数:无参  
+返回:返回true即为调试模式，返回false即为发布模式  
+异常:this指针非唯一的对象抛出异常std::safe\_alloc::this\_not\_true  

**7.**`free`**类内函数模板**  
+声明:`template<class T> void free(T* space);`  
+作用:释放堆区空间  
+参数:space想要释放的空间  
+模板:T要释放的空间数据类型  
+返回:无返回值  
+异常:this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  
------释放失败抛出异常safe\_alloc::bad\_free

**8.**`alloc`**类内函数模板**  
+声明:`template<class T> T* alloc(size_t size);`  
+作用:申请堆区空间  
+参数:size申请的个数  
------Debug特有:还需补充一个参数FileAndLine(实际上是`char* file,size_t line`两个参数)  
+模板:T所申请空间的类型  
+返回:申请的空间  
+异常:this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  
------申请失败抛出异常bad_alloc  
