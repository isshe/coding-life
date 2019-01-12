#include "ConcreteDecoratorB.h"

ConcreteDecoratorB::ConcreteDecoratorB(Component *component)
{
    _component = component;
}

std::string ConcreteDecoratorB::Operation()
{
    return _component->Operation() + ", "
    + "ConcreteDecoratorB::Operation()" + ": "
    + addOperation();
}

std::string ConcreteDecoratorB::addOperation()
{
    return "2";
}
