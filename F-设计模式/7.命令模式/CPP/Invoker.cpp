#include "Invoker.h"

void Invoker::executeAll() {
    _command->execute();
}

void Invoker::setCommand(Command *com) {
    _command = com;
}
