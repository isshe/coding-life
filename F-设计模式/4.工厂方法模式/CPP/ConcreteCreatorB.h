#include "Product.h"
#include "Creator.h"

class ConcreteCreatorB : public Creator {
public:
    Product* createProduct(int type);
};
