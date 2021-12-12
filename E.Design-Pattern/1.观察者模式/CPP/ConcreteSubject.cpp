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
