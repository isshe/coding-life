//#ifndef _CONCRETEPRODUCTA_H_
//#define _CONCRETEPRODUCTA_H_

#include "ConcreteProductA.h"
#include <iostream>

ConcreteProductA::ConcreteProductA() {
    _name = "ConcreteProductA";
}

std::string ConcreteProductA::getName() {
    return _name;
}

//#endif