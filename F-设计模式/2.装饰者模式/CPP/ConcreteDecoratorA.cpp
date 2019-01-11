#include "ConcreteDecoratorA.h"

 ConcreteDecoratorA::ConcreteDecoratorA(Component *component)
 {
     _pcomponent = component;
 }

 string ConcreteDecoratorA::Operation()
 {
     return "ConcreteDecoratorA::Operation()"
 }

 string ConcreteDecoratorA::addOperation()
 {
     _pcomponent->Operation() + ", " + this->Operation();
 }