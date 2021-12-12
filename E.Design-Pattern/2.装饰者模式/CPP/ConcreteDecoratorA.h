#include "Decorator.h"
#include "Component.h"
#include <iostream>

class ConcreteDecoratorA : public Decorator {
public:
    ConcreteDecoratorA(Component *component);
    std::string Operation();
    std::string addOperation();
private:
    Component *_component;
};