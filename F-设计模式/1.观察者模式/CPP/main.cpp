#include "ConcreteSubject.h"
#include "ConcreteObserver.h"

int main()
{
    ConcreteSubject *cs = new ConcreteSubject();
    ConcreteObserver *co = new ConcreteObserver(cs);

    cs->statusChanged();
    cs->statusChanged();

    return 0;
}