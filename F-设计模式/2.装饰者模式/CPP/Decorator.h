#ifndef _DECORATOR_H_
#define _DECORATOR_H_

#include "Component.h"

class Decorator : public Component {
public:
    //string Operation();
    virtual string addOperation() = 0;
}

#endif