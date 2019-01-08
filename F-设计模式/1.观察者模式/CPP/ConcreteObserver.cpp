
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
    //_subject = null;
}

void ConcreteObserver::update()
{
    std::cout << "update---1---" << std::endl;
}
