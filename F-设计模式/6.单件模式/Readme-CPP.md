[TOC]

# 单件模式-C++代码示例

## 1. 文件列表
* CPP1/:
    * main.cpp: 测试程序。
    * Singleton.h/cpp: 单件类。
* CPP2/: 抽象工厂类负责创建具体工厂类，同一时间只存在一个工厂。
    * main.cpp: 测试程序。
    * MazeFactory.h/cpp: 单件类，工厂抽象类。
    * BombedMazeFactory.h/cpp: 具体工厂类。
    * EnchatedMazeFactory.h/cpp：具体工厂类。
    * > 这份代码参考四人帮的《设计模式》，但是不十分确定是否符合书本的意图。
    * > 这种用法，个人感觉十分难用。

## 2. 各个文件关键代码
### 2.1 CPP1
* Singleton.h/cpp
```cpp
// Singleton.h
class Singleton {
public:
    static Singleton* instance();
    void saySomething();
protected:
    Singleton() {};
private:
    static Singleton* _instance;
};

// Singleton.cpp
Singleton* Singleton::_instance = 0;

Singleton* Singleton::instance() {
    if (_instance == 0) {
        _instance = new Singleton();
    }

    return _instance;
}

void Singleton::saySomething() {
    std::cout << "aha..." << std::endl;
}
```

* main.cpp
```cpp
int main()
{
    Singleton::instance()->saySomething();
    return 0;
}
```

### 2.2 CPP2
* MazeFactory.h/cpp
```cpp
// MazeFactory.h
class MazeFactory {
public:
    static MazeFactory* instance(int type);
    void saySomething();
    ~MazeFactory();
protected:
    //mazeFactory();
private:
    static MazeFactory* _instance;
};

// MazeFactory.cpp
MazeFactory* MazeFactory::_instance = 0;

MazeFactory* MazeFactory::instance(int type) {
    if (_instance == 0) {
        if (type == 1) {
            _instance = new BombedMazeFactory();
        } else if (type == 2) {
            _instance = new EnchantedMazeFactory();
        } else {
            _instance = new MazeFactory();
        }
    } else {
        std::cout << "_instance != 0" << std::endl;
    }

    return _instance;
}

MazeFactory::~MazeFactory()
{
    _instance = 0;
}

void MazeFactory::saySomething()
{
    std::cout << "MazeFactory..." << std::endl;
}
```

* BombedMazeFactory.h/cpp
```cpp
// BombedMazeFactory.h
class BombedMazeFactory : public MazeFactory
{
public:
    void saySomething();
};

//BombedMazeFactory.cpp
void BombedMazeFactory::saySomething()
{
    std::cout << "BombedMazeFactory..." << std::endl;
}
```

* EnchantedMazeFactory.h/cpp
```cpp
// EnchantedMazeFactory.h
class EnchantedMazeFactory : public MazeFactory
{
public:
    void saySomething();
};

// EnchantedMazeFactory.cpp
void EnchantedMazeFactory::saySomething()
{
    std::cout << "EnchantedMazeFactory..." << std::endl;
}
```

* main.cpp
```cpp
int main()
{
    MazeFactory *mf = MazeFactory::instance(0);
    mf->saySomething();
    delete mf;

    // 还要知道类型，还要类型转换，这么用是不是很不和谐？（要求用户完全知道自己创建的是什么类型）
    BombedMazeFactory *bmf = (BombedMazeFactory *)MazeFactory::instance(1);
    bmf->saySomething();
    delete bmf;

    EnchantedMazeFactory *emf = (EnchantedMazeFactory *)MazeFactory::instance(2);
    emf->saySomething();
    delete emf;

    return 0;
}
```
* 注意看这个函数，获取对象的时候，都用了强制类型转换，
* 这种单件模式，依赖使用者对得到的类型足够了解。
* > 此用法是我个人的理解，如有错误，请指出。

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《设计模式：可复用面向对象软件的基础》

