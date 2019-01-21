#ifndef _ABSTRACTFACTORY_H_
#define _ABSTRACTFACTORY_H_

#include "AbstractProductA.h"
#include "AbstractProductB.h"

class AbstractFactory {
public:
    virtual AbstractProductA *createProductA() = 0;
    virtual AbstractProductB *createProductB() = 0;

    virtual ~AbstractFactory() {};
};

#endif