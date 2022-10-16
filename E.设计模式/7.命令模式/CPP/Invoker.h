
#include "Command.h"

class Invoker {
public:
    void executeAll();
    void setCommand(Command *com);

private:
    Command *_command;
};