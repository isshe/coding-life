#ifndef _CREATOR_H_
#define _CREATOR_H_

#include "Product.h"

class Creator {
public:
    Product *createProduct(int type);
};

#endif