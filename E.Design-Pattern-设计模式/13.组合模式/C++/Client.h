#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Component.h"

class Client {
public:
    Client(Component *allComposite);
    void printComposite();

private:
    Component *_allComposite;
};

#endif