
#include "Context.h"
#include "ConcreteStrategyA.h"
#include "ConcreteStrategyB.h"

int main(void) {

    ConcreteStrategyA *CSA = new ConcreteStrategyA();
    Context *context = new Context(CSA);
    context->ContextInterface();
    delete context;
    delete CSA;

    ConcreteStrategyB *CSB = new ConcreteStrategyB();
    context = new Context(CSB);
    context->ContextInterface();
    delete context;
    delete CSB;

    return 0;
}