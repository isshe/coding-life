[TOC]

# 抽象工厂模式-C++代码示例

## 1. 文件列表
* AbstractFactory.h：抽象工厂，声明所有具体工厂的接口。
* AbstractProductA.h: 抽象产品A（相当于类别，下面有多种产品）
* AbstractProductB.h: 抽象产品B（相当于类别，下面有多种产品）
* ConcreteFactory1.h/cpp: 第一个具体工厂
* ConcreteFactory2.h/cpp: 第二个具体工厂
* ConcreteProductA1.h/cpp: 抽象产品A下的一个产品。
* ConcreteProductA2.h/cpp: 抽象产品A下的另一个产品。
* ConcreteProductB1.h/cpp: 抽象产品B下的一个产品。
* ConcreteProductB2.h/cpp: 抽象产品B下的另一个产品。

## 2. 各个文件关键代码
### 2.1 抽象工厂
* AbstractFactory.h
```cpp
class AbstractFactory {
public:
    virtual AbstractProductA *createProductA() = 0;
    virtual AbstractProductB *createProductB() = 0;

    virtual ~AbstractFactory() {};
};

```

### 2.2 抽象产品
* AbstractProductA.h
```cpp
class AbstractProductA {
public:

    virtual ~AbstractProductA() {};
};

```
* AbstractProductB.h
```cpp
class AbstractProductB {
public:

    virtual ~AbstractProductB() {};
};

```

### 2.3 具体工厂
* ConcreteFactory1.h/cpp
```cpp
//ConcreteFactory1.h
class ConcreteFactory1 : public AbstractFactory {
public:
    AbstractProductA *createProductA();
    AbstractProductB *createProductB();
};

//ConcreteFactory1.cpp
AbstractProductA* ConcreteFactory1::createProductA()
{
    return new ConcreteProductA1();
}

AbstractProductB* ConcreteFactory1::createProductB()
{
    return new ConcreteProductB1();
}
```
* ConcreteFactory2.h/cpp
```cpp
//ConcreteFactory2.h
class ConcreteFactory2 : public AbstractFactory {
public:
    AbstractProductA *createProductA();
    AbstractProductB *createProductB();
};

//ConcreteFactory2.cpp
AbstractProductA* ConcreteFactory2::createProductA()
{
    return new ConcreteProductA2();
}

AbstractProductB* ConcreteFactory2::createProductB()
{
    return new ConcreteProductB2();
}
```

### 2.4 具体产品
* ConcreteProductA1.h/cpp
```cpp
//ConcreteProductA1.h
class ConcreteProductA1 : public AbstractProductA {
public:
    ConcreteProductA1();
};

//ConcreteProductA1.cpp
ConcreteProductA1::ConcreteProductA1() {
    std::cout << "ConcreteProductA1" << std::endl;
}

```
* ConcreteProductA2.h/cpp: 类似ConcreteProductA1.h/cpp
* ConcreteProductB1.h/cpp: 类似ConcreteProductA1.h/cpp
* ConcreteProductB2.h/cpp: 类似ConcreteProductA1.h/cpp

### 2.5 测试代码
* main.cpp
```cpp
int main()
{
    // 初始化两个工厂
    AbstractFactory *cf1 = new ConcreteFactory1();
    AbstractFactory *cf2 = new ConcreteFactory2();

    // 第一个工厂进行生产
    AbstractProductA *apa = cf1->createProductA();
    delete apa;

    AbstractProductB *apb = cf1->createProductB();
    delete apa;

    // 第二个工厂进行生产
    apa = cf2->createProductA();
    delete apa;

    apb = cf2->createProductB();
    delete apa;

    delete cf1;
    delete cf2;

    return 0;
}
```

## x. 疑问

## y. 拓展/总结
* 和工厂方法的区别在于，工厂使用了抽象类来统一工厂，工厂可能有多个。

## z. 参考

