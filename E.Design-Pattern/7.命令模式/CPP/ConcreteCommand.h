
#include "Command.h"
#include "Receiver.h"

class ConcreteCommand : public Command {
public:
    void execute();

private:
    Receiver *_receiver;
};