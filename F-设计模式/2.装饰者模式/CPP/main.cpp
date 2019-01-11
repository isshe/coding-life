
#include <iostream>
#include "ConcreteComponentA.h"
#include "ConcreteComponentB.h"
#include "ConcreteDecoratorA.h"
#include "ConcreteDecoratorB.h"

int main()
{
    ConcreteComponentA *cca = new ConcreteComponentA();
    std::cout << "cca: " << cca->Operation();

    ConcreteComponentB *ccb = new ConcreteComponentB();
    std::cout << "ccb: " << ccb->Operation();


    delete

    return 0;
}