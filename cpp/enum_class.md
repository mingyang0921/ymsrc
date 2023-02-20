# enum class


## 一、定义

枚举类型：是用户定义的若干枚举常量的集合，属于用户自定义的数据类型。
C++枚举包含两种：限定作用域枚举类型和不限定作用域枚举类型。
定义限定作用域的枚举类型的一般形式是使用enum class关键字

## 二、使用

```cpp
enum ImType{
        P2pMsg = 1,
        GroupMsg,
        ChannelMsg
};

enum class ImClassType {
        P2pClassMsg = 100,
        GroupClassMsg,
        ChannelClassMsg
};

int main()
{
        ImType myctype = P2pMsg;

		ImClassType myclass = ImClassType::P2pClassMsg;
        ImClassType myclass = P2pClassMsg;        

        return 0;
}
```
编译结果
```
test1.cpp:19:14: error: redeclaration of ‘ImClassType myclass’
   19 |  ImClassType myclass = P2pClassMsg;
      |              ^~~~~~~
test1.cpp:18:14: note: ‘ImClassType myclass’ previously declared here
   18 |  ImClassType myclass = ImClassType::P2pClassMsg;
      |              ^~~~~~~
test1.cpp:19:24: error: ‘P2pClassMsg’ was not declared in this scope; did you mean ‘ImClassType::P2pClassMsg’?
   19 |  ImClassType myclass = P2pClassMsg;
      |                        ^~~~~~~~~~~
      |                        ImClassType::P2pClassMsg
test1.cpp:9:2: note: ‘ImClassType::P2pClassMsg’ declared here
    9 |  P2pClassMsg = 100,
      |  ^~~~~~~~~~~
```

注释掉 ImClassType myclass = P2pClassMsg;  即可编译通过