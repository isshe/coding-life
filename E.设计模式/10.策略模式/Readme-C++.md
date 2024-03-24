[TOC]

# 策略模式-C++ 代码示例
此示例按照类图实现，以说明 main(客户) 和 ConcreteStrategy 是不解耦的。


## 1. 文件列表
* Context.h/cpp: 对应类图中 Context
* Strategy.h: 对应类图中 Strategy
* ConcreteStrategyA.h/cpp: 对应类图中 ConcreteStrategy
* ConcreteStrategyB.h/cpp: 对应类图中 ConcreteStrategy

## 2. 各个文件关键代码
### 2.1 Context
* Context.h
```cpp
#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Strategy.h"

class Context {
public:
    Context(Strategy *strategy);

    void ContextInterface();

private:
    Strategy *_strategy;

};

#endif
```

* Context.cpp
```cpp
#include "Context.h"

Context::Context(Strategy *strategy)
{
    _strategy = strategy;
}

void Context::ContextInterface()
{
    _strategy->AlgorithmInterface();
}


```

### 2.2 Strategy
* Strategy.h
```cpp
#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategy {
public:
    Strategy() {};
    virtual ~Strategy() {};
    virtual void AlgorithmInterface() = 0;
};

#endif
```

### 2.3 ConcreteStrategyA
* ConcreteStrategyA.h
```cpp
#ifndef CONCRETESTRATEGYA_H_
#define CONCRETESTRATEGYA_H_

#include "Strategy.h"

class ConcreteStrategyA : public Strategy {
public:
    ConcreteStrategyA(){};
    ~ConcreteStrategyA(){};
    void AlgorithmInterface();
};

#endif
```

* ConcreteStrategyA.cpp
```cpp
#include "ConcreteStrategyA.h"
#include <iostream>

void ConcreteStrategyA::AlgorithmInterface()
{
    std::cout << "ConcreteStrategyA::AlgorithmInterface" << std::endl;
}

```

### 2.4 ConcreteStrategyB
* ConcreteStrategyB.h
```cpp
#ifndef CONCRETESTRATEGYB_H_
#define CONCRETESTRATEGYB_H_

#include "Strategy.h"

class ConcreteStrategyB : public Strategy {
public:
    ConcreteStrategyB(){};
    ~ConcreteStrategyB(){};
    void AlgorithmInterface();
};

#endif
```

* ConcreteStrategyB.cpp
```cpp
#include "ConcreteStrategyB.h"
#include <iostream>

void ConcreteStrategyB::AlgorithmInterface()
{
    std::cout << "ConcreteStrategyB::AlgorithmInterface" << std::endl;
}

```


### 2.5 Test
* main.cpp
```cpp
#include "Context.h"
#include "ConcreteStrategyA.h"
#include "ConcreteStrategyB.h"

int main(void) {

    ConcreteStrategyA *CSA = new ConcreteStrategyA();
    Context *context = new Context(CSA);
    context->ContextInterface();
    delete context;
    delete CSA;

    ConcreteStrategyB *CSB = new ConcreteStrategyB();
    context = new Context(CSB);
    context->ContextInterface();
    delete context;
    delete CSB;

    return 0;
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《设计模式 可复用面向对象软件的基础》

