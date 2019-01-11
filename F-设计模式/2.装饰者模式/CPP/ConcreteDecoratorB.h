#include "Decorator.h"
#include "Component.h"

class ConcreteDecoratorB : public Decorator {
public:
    ConcreteDecoratorB(Component *component);
    string Operation();
    string addOperation();
private:
    Component *_pcomponent;
}