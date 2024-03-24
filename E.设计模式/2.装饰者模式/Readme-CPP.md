[TOC]

# 装饰者模式-C++ 代码示例
* 这部分代码直接参考装饰者模式类图实现，不举现实中的例子，个人感觉还是这样来得直接。
* 如有错误，请指出。

## 1. 文件列表及功能
* Component.h: 组件/被装饰者；抽象类
* Decorator.h: 装饰者；抽象类，继承自 Component
* ConcreteComponentA.h/cpp: 具体组件 A；继承自 Component，属于基础组件。
* ConcreteComponentB.h/cpp: 具体组件 B；同组件 A。
* comcreteDecoratorA.h/cpp: 具体装饰者 A；继承自 Decorator，用于装饰其他 (被装饰过，没被装饰过的) 组件。
* comcreteDecoratorB.h/cpp: 具体装饰者 B；同装饰者 A。
* main.cpp: 测试代码，主函数。

## 2. 各个文件关键代码
### 2.1 组件/被装饰者抽象类
抽象类，定义一组接口，不存储数据。
* Component.h
```cpp
class Component {
public:
    Component() {};
    // Declare destructors virtual in polymorphic(多态) base classes.
    virtual ~Component() {};
    virtual std::string Operation() = 0;
};
```

### 2.2 装饰者抽象类
* Decorator.h
```cpp
class Decorator : public Component {
public:
    // Declare destructors virtual in polymorphic(多态) base classes.
    virtual ~Decorator() {};
    virtual std::string addOperation() = 0;
};
```

### 2.3 具体组件/被装饰者
* ConcreteComponentA.h/cpp
```cpp
// ConcreteComponentA.h
class ConcreteComponentA : public Component {
public:
    std::string Operation();
};

// ConcreteComponentA.cpp
std::string ConcreteComponentA::Operation()
{
    return "ConcreteComponentA::Operation()";
}
```

* ConcreteComponentB.h/cpp
```cpp
// ConcreteComponentB.h
class ConcreteComponentB : public Component {
public:
    std::string Operation();
};

// ConcreteComponentB.cpp
std::string ConcreteComponentB::Operation()
{
    return "ConcreteComponentB::Operation()";
}
```

### 2.4 具体装饰者
* comcreteDecoratorA.h/cpp
```cpp
class ConcreteDecoratorA : public Decorator {
public:
    ConcreteDecoratorA(Component *component);
    std::string Operation();
    std::string addOperation();
private:
    Component *_component;
};

// comcreteDecoratorA.cpp
ConcreteDecoratorA::ConcreteDecoratorA(Component *component)
{
    _component = component;
}

std::string ConcreteDecoratorA::Operation()
{
    return _component->Operation() + ", "
    + "ConcreteDecoratorA::Operation()" + ": "
    + addOperation();
}

std::string ConcreteDecoratorA::addOperation()
{
    return "1";
}

```

* comcreteDecoratorB.h/cpp
```cpp
class comcreteDecoratorB : public Decorator {
public:
    comcreteDecoratorB(Component *component);
    std::string Operation();
    std::string addOperation();
private:
    Component *_component;
};

// comcreteDecoratorB.cpp
comcreteDecoratorB::comcreteDecoratorB(Component *component)
{
    _component = component;
}

std::string comcreteDecoratorB::Operation()
{
    return _component->Operation() + ", "
    + "comcreteDecoratorB::Operation()" + ": "
    + addOperation();
}

std::string comcreteDecoratorB::addOperation()
{
    return "1";
}
```

### 2.5 测试代码
* main.cpp
```cpp
int main()
{
    // 这里应该有内存泄漏，如何解决？
    Component *cca = new ConcreteComponentA();
    std::cout << "cca: " << cca->Operation() << std::endl;
    Component *ccaA = new ConcreteDecoratorA(cca);
    std::cout << "ccaA: " << ccaA->Operation() << std::endl;
    Component *ccaB = new ConcreteDecoratorB(ccaA);
    std::cout << "ccaB: " << ccaB->Operation() << std::endl;
    Component *ccaBB = new ConcreteDecoratorB(ccaB);
    std::cout << "ccaBB: " << ccaBB->Operation() << std::endl;
    delete cca;
    delete ccaA;
    delete ccaB;
    delete ccaBB;


    // 这里应该有内存泄漏，如何解决？
    Component *ccb = new ConcreteComponentB();
    std::cout << "ccb: " << ccb->Operation() << std::endl;
    Component *ccbA = new ConcreteDecoratorA(ccb);
    std::cout << "ccbA: " << ccbA->Operation() << std::endl;
    Component *ccbB = new ConcreteDecoratorB(ccbA);
    std::cout << "ccbB: " << ccbB->Operation() << std::endl;
    delete ccb;
    delete ccbA;
    delete ccbB;

    return 0;
}
```

## x. 疑问
* 如何更优雅地释放内存？

## y. 拓展/总结

## z. 参考
* [虚析构函数](https://www.zhihu.com/question/41538182)