#include "Singleton.h"
#include <iostream>

Singleton* Singleton::_instance = 0;

Singleton* Singleton::instance() {
    if (_instance == 0) {
        _instance = new Singleton();
    }

    return _instance;
}

void Singleton::saySomething() {
    std::cout << "aha..." << std::endl;
}