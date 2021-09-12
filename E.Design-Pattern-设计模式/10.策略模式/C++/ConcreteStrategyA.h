#ifndef CONCRETESTRATEGYA_H_
#define CONCRETESTRATEGYA_H_

#include "Strategy.h"

class ConcreteStrategyA : public Strategy {
public:
    ConcreteStrategyA(){};
    ~ConcreteStrategyA(){};
    void AlgorithmInterface();
};

#endif