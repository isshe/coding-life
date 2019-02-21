#ifndef _ADAPTER_H_
#define _ADAPTER_H_


#include "Adaptee.h"
#include "Target.h"

class Adapter : public Target {
public:
    Adaptee *_adaptee;

    Adapter(Adaptee *adaptee);

    void request();
};

#endif