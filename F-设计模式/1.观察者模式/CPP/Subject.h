#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include "Observer.h"

class Observer;

class Subject {
public:
    virtual void registerObserver(Observer *o);
    virtual void removeObserver(Observer *o);
    virtual void notifyObserver();
};

#endif