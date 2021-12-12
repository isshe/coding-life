#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <iostream>

class Component {
public:
    Component() {};
    // Declare destructors virtual in polymorphic(多态) base classes.
    virtual ~Component() {};
    virtual std::string Operation() = 0;
};

#endif