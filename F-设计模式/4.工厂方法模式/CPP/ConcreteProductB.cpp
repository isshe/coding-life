#ifndef _CONCRETEPRODUCTA_H_
#define _CONCRETEPRODUCTA_H_

#include "ConcreteProductB.h"
#include <iostream>

ConcreteProductB::ConcreteProductB() {
    _name = "ConcreteProductB";
}

std::string ConcreteProductB::getName() {
    return _name;
}

#endif