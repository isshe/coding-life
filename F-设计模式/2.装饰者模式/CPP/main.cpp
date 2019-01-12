
#include <iostream>
#include "Component.h"
#include "ConcreteComponentA.h"
#include "ConcreteComponentB.h"
#include "ConcreteDecoratorA.h"
#include "ConcreteDecoratorB.h"

int main()
{
    // 这里应该有内存泄漏，如何解决？
    Component *cca = new ConcreteComponentA();
    std::cout << "cca: " << cca->Operation() << std::endl;
    Component *ccaA = new ConcreteDecoratorA(cca);
    std::cout << "ccaA: " << ccaA->Operation() << std::endl;
    Component *ccaB = new ConcreteDecoratorB(ccaA);
    std::cout << "ccaB: " << ccaB->Operation() << std::endl;
    Component *ccaBB = new ConcreteDecoratorB(ccaB);
    std::cout << "ccaBB: " << ccaBB->Operation() << std::endl;
    delete cca;
    delete ccaA;
    delete ccaB;
    delete ccaBB;


    // 这里应该有内存泄漏，如何解决？
    Component *ccb = new ConcreteComponentB();
    std::cout << "ccb: " << ccb->Operation() << std::endl;
    Component *ccbA = new ConcreteDecoratorA(ccb);
    std::cout << "ccbA: " << ccbA->Operation() << std::endl;
    Component *ccbB = new ConcreteDecoratorB(ccbA);
    std::cout << "ccbB: " << ccbB->Operation() << std::endl;
    delete ccb;
    delete ccbA;
    delete ccbB;

    return 0;
}