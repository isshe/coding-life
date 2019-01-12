#include "Decorator.h"
#include "Component.h"
#include <iostream>

class ConcreteDecoratorB : public Decorator {
public:
    ConcreteDecoratorB(Component *component);
    std::string Operation();
    std::string addOperation();
private:
    Component *_component;
};