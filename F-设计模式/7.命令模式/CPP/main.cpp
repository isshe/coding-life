#include <iostream>
#include "ConcreteCommand.h"
#include "Invoker.h"
#include "Receiver.h"


int main()
{
    Invoker *invoker = new Invoker();
    Receiver *receiver = new Receiver();
    ConcreteCommand *cc = new ConcreteCommand();

    invoker->setCommand(cc);
    invoker->executeAll();

    return 0;
}