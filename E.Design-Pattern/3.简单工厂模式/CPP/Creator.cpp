#include "Creator.h"
#include "ProductA.h"
#include "ProductB.h"
#include <iostream>

Product *Creator::createProduct(int type)
{
    Product *p = NULL;
    if (type == 1) {
        p = new ProductA();
    } else {
        p = new ProductB();
    }

    return p;
}