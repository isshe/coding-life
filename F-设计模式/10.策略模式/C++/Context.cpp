#include "Context.h"

Context::Context(Strategy *strategy)
{
    _strategy = strategy;
}

void Context::ContextInterface()
{
    _strategy->AlgorithmInterface();
}

