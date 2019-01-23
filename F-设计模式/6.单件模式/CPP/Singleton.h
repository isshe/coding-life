#ifndef _SINGLETON_H_
#define _SINGLETON_H_

class Singleton {
public:
    static Singleton* instance();
    void saySomething();
protected:
    Singleton() {};
private:
    static Singleton* _instance;
};

#endif