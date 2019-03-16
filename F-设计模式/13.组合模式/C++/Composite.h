#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include "Component.h"
#include <iostream>
#include <vector>

using namespace std;

class Composite : public Component {
public:
    void add(Component *component);
    void print();

private:
    vector<Component *> _items;
};


#endif