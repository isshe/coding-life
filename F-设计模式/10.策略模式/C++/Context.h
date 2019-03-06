#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Strategy.h"

class Context {
public:
    Context(Strategy *strategy);

    void ContextInterface();

private:
    Strategy *_strategy;

};

#endif