# func delete

## 一、原理
C++11中，当我们定义一个类的成员函数时，如果后面使用"=delete"去修饰，那么就表示这个函数被定义为deleted，也就意味着这个成员函数不能再被调用，否则就会出错。

## 二、代码示例

```cpp
#include <cstdio>
class TestClass
{
public:
    void func(int data) { printf("data: %d\n", data); }
};
int main(void)
{
    TestClass obj;
    obj.func(100);
    obj.func(100.0);
    return 0;
}
```

编译结果
```cpp
ym@LAPTOP-O2UV9NP9:~$ g++ test.cpp
test.cpp: In function ‘int main()’:
test.cpp:12:19: error: use of deleted function ‘void TestClass::func(double)’
   12 |     obj.func(100.0);
      |                   ^
test.cpp:6:10: note: declared here
    6 |     void func(double data)=delete;
      |          ^~~~
```