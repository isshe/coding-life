#ifndef _DECORATOR_H_
#define _DECORATOR_H_

#include "Component.h"
#include <iostream>

class Decorator : public Component {
public:
    // Declare destructors virtual in polymorphic(多态) base classes.
    virtual ~Decorator() {};
    virtual std::string addOperation() = 0;
};

#endif