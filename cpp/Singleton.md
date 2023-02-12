# Singleton 模式（单例模式）

## 一、实现步骤
1. 构造函数私有化
2. 增加静态私有的当前类的指针变量
3. 提供静态对外接口,可以让用户获得单例对象

## 二、类型分类
1. 懒汉式：解决了饿汉式内存浪费问题，但是线程不安全的，可以通过互斥量mutex.lock()和mutex.unlock()来解决
2. 饿汉式：还没有使用该单例对象，该单例对象就已经被加载到内存了，在对象过多时会造成内存浪费


## 三、代码

### 3.1 懒汉模式

```cpp
class Singleton_lazy{
private:
	Singleton_lazy(){
		cout<<"我是懒汉式构造函数"<<endl;
	}
public：
	static Singleton_lazy* getInstance(){
		if(pSingleton == NULL){
			pSingleton = new Singleton_lazy;
		}
		return pSingleton;
	}
private:
	static Singleton_lazy* pSingleton;
};
//类外初始化
Singleton_lazy* Singleton_lazy::pSingleton = NULL;

```

### 3.2 饿汉模式
```cpp
class Singleton_hungry{
private:
	Singleton_hungry(){
		cout<<"我是饿汉式构造函数"<<endl;
	}
public：
	static Singleton_hungry* getInstance(){
		return pSingleton;
	}
private:
	static Singleton_hungry* pSingleton;
};
//类外初始化
Singleton_hungry* Singleton_hungry::pSingleton = new Singleton_hungry;

```

### 3.3 饿汉模式下自动运行
```cpp
int main(){
	cout << "main函数开始执行"<<endl;

	return 0;
}
```
//输出
我是饿汉式构造函数

### 3.4 相同地址空间

```cpp
int main(){
	//p1和p2指向同一地址空间，说明只创造出了一个懒汉式对象
	Singleton_lazy* p1 = Singleton_lazy::getInstance();
	Singleton_lazy* p2 = Singleton_lazy::getInstance();
	//p3和p4指向同一地址空间，说明只创造出了一个饿汉式对象
	Singleton_hungry* p3 = Singleton_hungry::getInstance();
	Singleton_hungry* p4 = Singleton_hungry::getInstance();
	return 0;
}

```
GDB发现相同的地址空间，实现单例
