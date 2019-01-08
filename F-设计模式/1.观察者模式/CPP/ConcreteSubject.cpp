#include <iostream>
#include <list>

#include "ConcreteSubject.h"

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
