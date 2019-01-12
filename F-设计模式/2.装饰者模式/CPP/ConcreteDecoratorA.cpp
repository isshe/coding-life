#include "ConcreteDecoratorA.h"

ConcreteDecoratorA::ConcreteDecoratorA(Component *component)
{
    _component = component;
}

std::string ConcreteDecoratorA::Operation()
{
    return _component->Operation() + ", "
    + "ConcreteDecoratorA::Operation()" + ": "
    + addOperation();
}

std::string ConcreteDecoratorA::addOperation()
{
    return "1";
}
