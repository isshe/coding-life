#ifndef _CONCRETECREATORA_H_
#define _CONCRETECREATORA_H_

#include "ConcreteProductB.h"
#include "ConcreteCreatorB.h"
#include <iostream>

Product* ConcreteCreatorB::createProduct(int type)
{
    if (type == 'B') {
        return new ConcreteProductB();
    }

    return NULL;
}

#endif