# c++指针指针

## 一、介绍

### 1.1 指针与智能指针的不同


#### 1.1.1 运行代码

```cpp
#include <string>
#include <iostream>
#include <memory>

using namespace std;

class Test {
public:
        Test(string name) {
                this->name = name;
                cout << "Test的构造函数..." << name  << endl;
        }
        ~Test() { cout << "Test的析构函数..." << name << endl;}
        string UpdateName(string name){
                this->name = name;
                return this->name;
        }
private:
        string name;;
};


int main(void) {

        Test *test = new Test("point");

        auto_ptr<Test> t(new Test("auto_ptr"));
        unique_ptr<Test> t1(new Test("unique_ptr"));
        shared_ptr<Test> t2(new Test("shared_ptr"));
        weak_ptr<Test> t3(t2);

        return 0;
}
```
#### 1.1.2 运行结果
```
ym@LAPTOP-O2UV9NP9:~/demo/point$ ./a.out
Test的构造函数...point
Test的构造函数...auto_ptr
Test的构造函数...unique_ptr
Test的构造函数...shared_ptr
Test的析构函数...shared_ptr
Test的析构函数...unique_ptr
Test的析构函数...auto_ptr
```
#### 1.1.3 结果分析
1. 普通指针不会自动释放
2. 智能指针可以自动释放

### 1.2 auto_ptr

#### 1.2.1 运行代码

```cpp
#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

using std::string;
using std::auto_ptr;

int main()
{
        //弊端1：复制或赋值都会改变资源的所有权
        auto_ptr<string> p1(new string("I 'm string1."));
        auto_ptr<string> p2(new string("I 'm string2."));
        printf("p1: %p\n", p1.get());
        printf("p2: %p\n", p2.get());

        p1 = p2;        //代码1
        printf("after p1 = p2\n");
        printf("p1: %p\n", p1.get());
        printf("p2: %p\n", p2.get());

        return 0;
}
```
#### 1.2.2 运行结果

```
ym@LAPTOP-O2UV9NP9:~/demo/point$ ./a.out
p1: 0x7fffe5f4feb0
p2: 0x7fffe5f4fee0
after p1 = p2
p1: 0x7fffe5f4fee0
p2: (nil)
```
#### 1.2.3 结果分析
auto_ptr拥有所指资源的独占所有权，复制或赋值后，所有权会被转移，这样会导致所有权混乱。

### 1.3 unique_ptr

#### 1.3.1 运行代码
```cpp
#include <string>
#include <iostream>
#include <memory>

using namespace std;

int main()
{
        unique_ptr<string> p(new string("I Love China"));
        unique_ptr<string> q = std::move(p);    //移动后p指针为空
        return 0;
}
```
#### 1.3.2 运行结果
可以编译通过
#### 1.3.3 结果分析
独占式指针(专属所有权)，同一时刻，智能有一个unique_ptr指针指向这个对象(内存)。
### 1.4 shared_ptr

#### 1.4.1 运行代码
```cpp
#include <string>
#include <iostream>
#include <memory>

using namespace std;

void test(shared_ptr<int> p) {
        ++*p;
        cout <<"值传递时，引用计数加一："<< p.use_count() << endl;//2
}
int main(void) {
        int * a = new int(999);
        shared_ptr<int> p1(a);

        cout << *p1 << endl;//999
        cout << p1.use_count() << endl;//2

        test(p1);
        cout << *p1 << endl;//1000
        cout << p1.use_count() << endl;//1

        return 0;
}
```
#### 1.4.2 运行结果
```
ym@LAPTOP-O2UV9NP9:~/demo/point$ ./a.out
999
1
值传递时，引用计数加一：2
1000
1
```
#### 1.4.3 结果分析
shared_ptr允许多个指针指向同一个对象,且增加了引用计数，当引用计数为0时，才进行销毁

### 1.5 weak_ptr

#### 1.5.1 运行代码

```cpp
#include <iostream>
#include <memory>

class Test {
public:
    Test() {
        std::cout << "Test构造函数" << std::endl;
    }
    ~Test() {
        std::cout << "Test析构函数" << std::endl;
    }

    void print() {
        std::cout << "Test的成员函数" << std::endl;
    }
};

int main() {
    std::weak_ptr<Test> wp;
    {
        std::shared_ptr<Test> sp(new Test);
        wp = sp;
    }

    std::shared_ptr<Test> sp = wp.lock();
    if (sp) {
        sp->print();
    } else {
        std::cout << "weak_ptr指向的对象已经被销毁" << std::endl;
    }

    return 0;
}
```

#### 1.5.2 运行结果
```
ym@LAPTOP-O2UV9NP9:~/demo/point$ ./a.out
Test构造函数
Test析构函数
weak_ptr指向的对象已经被销毁
```

#### 1.5.3 结果分析
为了防止share_ptr互相引用导致无法销毁,增加weak_ptr引用，当时用weak_ptr引用时不增加引用计数，同事如果要使用weak_ptr需要lock转换,如果已经释放了,返回结果为NULL。

## 二、总结
1. 智能指针可以自动释放,解决普通指针容易忘记释放内存泄露问题。
2. auto_ptr有独占权，复制后会转移,容易出现混乱
3. unique_ptr禁止复制转移，解决容易混乱问题
4. share_ptr解决独占权问题，增加互相引用问题
5. weak_ptr解决了互相引用问题