
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
    _subject = NULL;
}

void ConcreteObserver::update()
{
    std::cout << "update status: " << _subject->getStatus() << std::endl;
}
