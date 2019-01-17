#ifndef _CREATOR_H_
#define _CREATOR_H_

#include "Product.h"

class Creator {
public:
    virtual ~Creator() {};
    virtual Product *createProduct(int type) = 0;
};

#endif