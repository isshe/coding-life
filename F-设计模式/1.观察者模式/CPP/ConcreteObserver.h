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