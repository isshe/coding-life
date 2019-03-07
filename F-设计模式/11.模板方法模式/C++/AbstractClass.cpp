
#include "AbstractClass.h"
#include <iostream>

void AbstractClass::templateMethod() {
    std::cout << "AbstractClass::templateMethod---" << std::endl;
    primitiveOperation1();
    primitiveOperation2();
}