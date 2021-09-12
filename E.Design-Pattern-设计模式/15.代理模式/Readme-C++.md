[TOC]

# 代理模式-C++示例

## 1. 文件列表
* Subject.h: 主题，抽象类，定义主题的接口。
* RealSubject.h/cpp: 具体的主题，继承自Subject。
* Proxy.h/cpp: 代理，代理RealSubject，继承自Subject。
* main.cpp: 测试代码

## 2. 各个文件关键代码
### 2.1 主题及具体主题
* Subject.h: 主题，抽象类，定义主题的接口。
```cpp
class Subject {
public:
    virtual void request() = 0;
};
```

* RealSubject.h
```cpp
class RealSubject : public Subject {
public:
    virtual void request();
};
```

* RealSubject.cpp
```cpp
void RealSubject::request()
{
    cout << "RealSubject::request()" << endl;
}
```

### 2.2 代理
* Proxy.h
```cpp
class Proxy : public Subject {
public:
    Proxy();
    ~Proxy();
    virtual void request();

private:
    RealSubject *_realSubject;
};

```

* Proxy.cpp
```cpp
Proxy::Proxy()
{
    _realSubject = new RealSubject();
}

Proxy::~Proxy()
{
    delete _realSubject;
}


void Proxy::request()
{
    _realSubject->request();
}
```

### 2.3 测试代码
* main.cpp: 测试代码
```cpp
int main()
{
    Proxy px;

    px.request();
    return 0;
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考

