#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <iostream>

class Product {
public:
    virtual ~Product() {};
    virtual std::string getName() = 0;
};

#endif