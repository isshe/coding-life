
#include "Product.h"
#include "ConcreteCreatorA.h"
#include "ConcreteCreatorB.h"
#include <iostream>

int main()
{
    ConcreteCreatorA cca;
    ConcreteCreatorB ccb;

    Product *product = cca.createProduct('A');
    std::cout << product->getName() << std::endl;
    delete product;
    product = NULL;

    product = ccb.createProduct('B');
    std::cout << product->getName() << std::endl;
    delete product;
    product = NULL;

    return 0;
}