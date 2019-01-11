#include "ConcreteDecoratorB.h"

ConcreteDecoratorB::ConcreteDecoratorB(Component *component)
{
    _pcomponent = component;
}

string ConcreteDecoratorB::Operation()
{
    return "ConcreteDecoratorB::Operation()"
}

string ConcreteDecoratorB::addOperation()
{
    _pcomponent->Operation() + ", " + this->Operation();
}