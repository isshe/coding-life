#ifndef _CONCRETECREATORA_H_
#define _CONCRETECREATORA_H_

#include "ConcreteCreatorA.h"
#include "ConcreteProductA.h"
#include <iostream>

Product* ConcreteCreatorA::createProduct(int type)
{
    if (type == 'A') {
        return new ConcreteProductA();
    }

    return NULL;
}

#endif