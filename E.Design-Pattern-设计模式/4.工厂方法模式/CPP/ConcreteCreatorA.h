
#include "Product.h"
#include "Creator.h"

class ConcreteCreatorA : public Creator {
public:
    Product* createProduct(int type);
};
