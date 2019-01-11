#include "Decorator.h"
#include "Component.h"

class ConcreteDecoratorA : public Decorator {
public:
    ConcreteDecoratorA(Component *component);
    string Operation();
    string addOperation();
private:
    Component *_pcomponent;
}