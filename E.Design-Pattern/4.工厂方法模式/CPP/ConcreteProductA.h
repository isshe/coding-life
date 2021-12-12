#include <iostream>
#include "Product.h"

class ConcreteProductA : public Product {
public:
    ConcreteProductA();
    std::string getName();
private:
    std::string _name;
};
