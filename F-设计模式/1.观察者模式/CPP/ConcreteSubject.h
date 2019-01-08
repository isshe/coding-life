#ifndef _CONCRETE_SUBJECT_H_
#define _CONCRETE_SUBJECT_H_

#include <list>
#include "Subject.h"
#include "Observer.h"

class ConcreteSubject : public Subject {
public:
    void registerObserver(Observer *o);
    void removeObserver(Observer *o);
    void notifyObservers();

private:
    std::list<Observer *> _observers;
};


#endif