#include "Creator.h"

int main()
{
    Creator c;

    c.createProduct(1)->operation();
    c.createProduct(0)->operation();

    return 0;
}