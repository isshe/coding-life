#ifndef _PROXY_H_
#define _PROXY_H_

#include "Subject.h"
#include "RealSubject.h"

class Proxy : public Subject {
public:
    Proxy();
    ~Proxy();
    virtual void request();

private:
    RealSubject *_realSubject;
};

#endif

