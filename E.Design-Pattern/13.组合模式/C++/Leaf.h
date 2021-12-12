#ifndef _LEAF_H_
#define _LEAF_H_

#include "Component.h"

using namespace std;

class Leaf : public Component {
public:
    void add(Component *component);
    void print();
};

#endif