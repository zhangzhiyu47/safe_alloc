# safe_alloc - C++智能内存分配与追踪库

## 目录
- [1. 项目概述](#1-项目概述)
- [2. 功能特性](#2-功能特性)
- [3. 安装指南](#3-安装指南)
    - [3.1 手动安装](#31-手动安装)
    - [3.2 使用CMake安装](#32-使用cmake安装)
- [4. 使用说明](#4-使用说明)
    - [4.1 基本使用流程](#41-基本使用流程)
    - [4.2 模式选择](#42-模式选择)
    - [4.3 核心API](#43-核心api)
- [5. 示例代码](#5-示例代码)
- [6. 异常处理](#6-异常处理)
- [7. 跨平台支持](#7-跨平台支持)
- [8. 注意事项](#8-注意事项)
- [9. 卸载指南](#9-卸载指南)
- [10. 贡献与许可](#10-贡献与许可)

## 1. 项目概述

**safe_alloc**是一个基于C++ STL和GCC开发的智能内存分配与追踪库，采用单例模式设计。该库提供了安全的内存分配/释放机制，并能在调试模式下提供详细的内存使用信息追踪。

**作者**: 张志宇  
**许可证**: [查看LICENSE文件](LICENSE)  
**特点**:
- 防止非法内存访问
- 内存泄漏检测
- 调试/发布双模式
- 跨平台支持(Windows/Linux/macOS/Termux)

## 2. 功能特性

- **内存安全**: 检测非法this指针和无效内存访问
- **双工作模式**:
    - **Release模式**: 高性能，最小内存开销
    - **Debug模式**: 完整内存追踪信息
- **内存信息追踪**:
    - 分配位置(文件名/行号)
    - 分配大小
    - 变量名信息
- **异常机制**: 提供详细的错误反馈
- **单例设计**: 确保全局唯一内存管理器

## 3. 安装指南

### 3.1 手动安装

```bash
# 复制头文件到系统目录
sudo cp source/safe_alloc /usr/include/  # Linux/macOS
copy source\safe_alloc "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\include\"  # Windows
cp source/safe_alloc $PREFIX/include/  # Termux
```

### 3.2 使用CMake安装

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local  # 自定义安装路径
cmake --build .
cmake --install .  # 可能需要管理员权限
```

## 4. 使用说明

### 4.1 基本使用流程

1. **包含头文件**:
   ```cpp
   #include <safe_alloc>  // 系统目录安装
   // 或
   #include "safe_alloc"  // 本地目录
   ```

2. **初始化单例** (***全局只需一次***):
   ```cpp
   init_safe_alloc;  // 必须在使用前调用
   ```

3. **使用分配器**:
   ```cpp
   int* arr = std::salloc.alloc<int>(10);  // 分配10个int
   std::salloc.free(arr);  // 释放内存
   ```

### 4.2 模式选择

```cpp
#define _DEBUG  // 启用Debug模式(必须在include前，且每个文件都需要)
#include <safe_alloc>
```

或通过编译器参数:
```bash
g++ -D_DEBUG your_program.cpp -o your_program
```

### 4.3 核心API

- **1.**`safe_alloc::space_info`**空间信息类**  
    - size_t num;申请的数量  
    - char * name;申请的变量名  
    - 以下为Debug模式独有  
        - char * file;申请代码所在的文件名  
        - size_t size;申请变量的单个字节数  
        - size_t line;申请代码所在的行数  

- **2.**`FileAndLine`**宏定义**  
    - 定义:`#define FileAndLine`  
    - 作用:直接用于参数结尾为char* file,size_t line的函数中，直接FileAndLine一个参数即可  

- **3.**`get_self`**类内方法**  
    - 声明:`safe_alloc& get_self(void);`  
    - 作用:获取单例模式的唯一对象  
    - 参数:无参  
    - 返回:safe_alloc&  

- **4.**`information`**类内方法**  
    - 声明:`pair<bool,safe_alloc::space_info> information(void *space) const;`  
    - 作用:查找space空间的信息  
    - 参数:space要查找的空间  
    - 返回:pair<bool,safe\_alloc::space\_info>，bool为查找是否成功  
    - 异常:this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  

- **5.**`number`**类内方法**  
    - 声明:`size_t number(void) const;`  
    - 作用:获取未释放的申请次数  
    - 返回:未释放的申请次数  
    - 异常:this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  

- **6.**`is_debug`**类内方法**  
    - 声明:`bool is_debug(void) const;`  
    - 作用:判断是否为调试模式  
    - 参数:无参  
    - 返回:返回true即为调试模式，返回false即为发布模式  
    - 异常:this指针非唯一的对象抛出异常std::safe\_alloc::this\_not\_true  

- **7.**`free`**类内函数模板**  
    - 声明:`template<class T> void free(T* space);`  
    - 作用:释放堆区空间  
    - 参数:space想要释放的空间  
    - 模板:T要释放的空间数据类型  
    - 返回:无返回值  
    - 异常:
        - this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  
        - 释放失败抛出异常safe\_alloc::bad\_free

- **8.**`alloc`**类内函数模板**  
    - 声明:`template<class T> T* alloc(size_t size);`  
    - 作用:申请堆区空间  
    - 参数:size申请的个数  
        - Debug特有:还需补充一个参数FileAndLine(实际上是`char* file,size_t line`两个参数)  
    - 模板:T所申请空间的类型  
    - 返回:申请的空间  
    - 异常:this指针非唯一的对象抛出异常safe\_alloc::this\_not\_true  
        - 申请失败抛出异常bad_alloc

## 5. 示例代码

[\[try/main.c\]](try/main.c)

## 6. 异常处理

| 异常类型 | 触发条件 |
|----------|----------|
| `this_not_true` | 非法this指针 |
| `bad_alloc` | 内存分配失败 |
| `bad_free` | 内存释放失败 |

**处理示例**:
```cpp
try {
    int* p = std::salloc.alloc<int>(100);
    std::salloc.free(p);
} catch(const std::safe_alloc::this_not_true& e) {
    std::cerr << "Invalid singleton access!" << std::endl;
} catch(const std::bad_alloc& e) {
    std::cerr << "Memory allocation failed!" << std::endl;
}
```

## 7. 跨平台支持

- **Windows**: 支持Visual Studio各版本
- **Linux/macOS**: 支持GCC/Clang
- **Termux**: Android终端环境支持

## 8. 注意事项

1. **初始化要求**:
    - `init_safe_alloc`***必须且只能调用一次***
    - 多文件项目中应在主源文件中初始化

2. **模式一致性**:
    - 确保所有源文件使用相同模式(***都定义或都不定义***`_DEBUG`)

3. **内存管理**:
    - 必须使用`safe_alloc`分配和释放内存
    - 不要混合使用malloc/free和safe_alloc

## 9. 卸载指南

```bash
# CMake方式
cd build
cmake --build . --target uninstall
```

```bash
# 手动删除
sudo rm /usr/include/safe_alloc  # Linux/macOS
del "C:\Program Files\safe_alloc\include\safe_alloc"  # Windows
rm $PREFIX/include/safe_alloc  # Termux
```

## 10. 贡献与许可

- **源码修改**: 不建议随意修改核心代码
- **问题报告**: 欢迎提交issue
- **许可协议**: 详见[LICENSE](LICENSE)文件

**重要**: 使用本库即表示您同意遵守许可条款([LICENSE](LICENSE))。商业使用前请确认许可权限。
