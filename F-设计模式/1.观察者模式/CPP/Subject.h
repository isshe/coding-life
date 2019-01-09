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