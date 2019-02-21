
#include "Adapter.h"
#include <iostream>

Adapter::Adapter(Adaptee *adaptee)
{
    _adaptee = adaptee;
}

void Adapter::request()
{
    //std::cout << "Adapter::request" << std::endl;
    _adaptee->specificRequest();
}
