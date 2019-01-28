
#include "ConcreteCommand.h"

void ConcreteCommand::execute() {
    _receiver->Action();
}
