#include <iostream>
#include "Product.h"

class ConcreteProductB : public Product {
public:
    ConcreteProductB();
    std::string getName();
private:
    std::string _name;
};
