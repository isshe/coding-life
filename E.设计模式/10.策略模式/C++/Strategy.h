#ifndef STRATEGY_H_
#define STRATEGY_H_

class Strategy {
public:
    Strategy() {};
    virtual ~Strategy() {};
    virtual void AlgorithmInterface() = 0;
};

#endif