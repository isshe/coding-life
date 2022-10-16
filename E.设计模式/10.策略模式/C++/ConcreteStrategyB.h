#ifndef CONCRETESTRATEGYB_H_
#define CONCRETESTRATEGYB_H_

#include "Strategy.h"

class ConcreteStrategyB : public Strategy {
public:
    ConcreteStrategyB(){};
    ~ConcreteStrategyB(){};
    void AlgorithmInterface();
};

#endif