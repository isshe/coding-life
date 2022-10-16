#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class Component {
public:
    virtual void add(Component *component) = 0;
    virtual void print() = 0;
};

#endif