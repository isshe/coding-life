
#include "Proxy.h"

Proxy::Proxy()
{
    _realSubject = new RealSubject();
}

Proxy::~Proxy()
{
    delete _realSubject;
}


void Proxy::request()
{
    _realSubject->request();
}
