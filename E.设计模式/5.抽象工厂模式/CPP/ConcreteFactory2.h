#include "AbstractFactory.h"
#include "AbstractProductA.h"
#include "AbstractProductB.h"

class ConcreteFactory2 : public AbstractFactory {
public:
    AbstractProductA *createProductA();
    AbstractProductB *createProductB();
};