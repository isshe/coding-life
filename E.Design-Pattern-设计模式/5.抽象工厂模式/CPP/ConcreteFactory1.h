#include "AbstractFactory.h"
#include "AbstractProductA.h"
#include "AbstractProductB.h"

class ConcreteFactory1 : public AbstractFactory {
public:
    AbstractProductA *createProductA();
    AbstractProductB *createProductB();
};