[TOC]

# 观察者模式-C++实现示例
* 此示例使用`拉`的方式。
* 示例描述：
    * `具体主题`状态变更时，通知所有`具体观察者`
    
## 1. 文件列表
* Subject.h: 主题，类里的函数实现为纯虚函数，提供类似Java中`interface`的功能。
* Observer.h：观察者，类里的函数实现为纯虚函数，提供类似Java中`interface`的功能。
* ConcreteSubject.h/cpp: 具体的主题
* ConcreteObserver.h/cpp: 具体的观察者
* main.java: 测试程序

## 2. 主题
提供`注册`、`取消注册`、`通知`、`获取状态`接口。
注意：getStatus()这里计划是放具体主题的类里面的，无奈当前不懂其他实现。

* Subject.h
```cpp
#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include "Observer.h"

class Observer;

class Subject {
public:
    virtual void registerObserver(Observer *o) = 0;
    virtual void removeObserver(Observer *o) = 0;
    virtual void notifyObservers() = 0;
    virtual int getStatus() = 0; // 这个原来只打算放到具体主题里面的，但是会出现具体观察者无法调用的情况
};

#endif
```

## 3. 观察者
提供`主题`更新后更新`观察者`的update接口。
* Observer.h
```cpp
#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Observer {
public:
    virtual void update() = 0;
};

#endif
```

## 4. 具体的主题
* 继承自`Subject`，并实现一些自己的函数。
* ConcreteSubject.h
```cpp
#ifndef _CONCRETE_SUBJECT_H_
#define _CONCRETE_SUBJECT_H_

#include <list>
#include "Subject.h"
#include "Observer.h"

class ConcreteSubject : public Subject {
public:
    ConcreteSubject();
    void registerObserver(Observer *o);
    void removeObserver(Observer *o);
    void notifyObservers();
    void statusChanged()
    int getStatus();

private:
    std::list<Observer *> _observers;
    int _status;
};


#endif
```

* ConcreteSubject.cpp
```cpp
#include <iostream>
#include <list>

#include "ConcreteSubject.h"

ConcreteSubject::ConcreteSubject()
{
    _status = 0;
}

void ConcreteSubject::registerObserver(Observer *o)
{
    _observers.push_back(o);
}

void ConcreteSubject::removeObserver(Observer *o)
{
    if (_observers.size() > 0) {
        _observers.remove(o);
    }
}

void ConcreteSubject::notifyObservers()
{
    std::list<Observer*>::iterator it;
    for (it = _observers.begin(); it != _observers.end(); it++) {
        (*it)->update();
    }
}

void ConcreteSubject::statusChanged()
{
    _status++;
    notifyObservers();
}

int ConcreteSubject::getStatus()
{
    return _status;
}

```

## 5. 具体的观察者
继承自`Observer`，并实现一些自己的函数。
* ConcreteObserver.h
```cpp
#ifndef _CONCRETE_OBSERVER_H_
#define _CONCRETE_OBSERVER_H_

#include "Observer.h"
#include "Subject.h"

class ConcreteObserver : public Observer {
public:
    ConcreteObserver(Subject *subject);
    ~ConcreteObserver();
    void update();

private:
    Subject *_subject;
};

#endif
```

* ConcreteObserver.cpp
构造函数里面进行了注册
```cpp
#include <iostream>
#include "ConcreteObserver.h"

ConcreteObserver::ConcreteObserver(Subject *subject)
{
    _subject = subject;
    subject->registerObserver(this);
}

ConcreteObserver::~ConcreteObserver()
{
    _subject->removeObserver(this);
    _subject = NULL;
}

void ConcreteObserver::update()
{
    std::cout << "update status: " << _subject->getStatus() << std::endl;
}

```

## 6. 测试程序
* main.cpp
```cpp
#include "ConcreteSubject.h"
#include "ConcreteObserver.h"

int main()
{
    // 创建具体的主题，具体的观察者
    ConcreteSubject *cs = new ConcreteSubject();
    ConcreteObserver *co = new ConcreteObserver(cs); // 构造函数里面进行了注册

    // 主题更新了
    cs->statusChanged();
    cs->statusChanged();

    return 0;
}
```


## x. 参考
* 《Head First设计模式》
* [纯虚函数](https://harttle.land/2015/06/28/cpp-polymorphism.html)




