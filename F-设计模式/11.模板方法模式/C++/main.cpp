
//#include "AbstractClass.h"
#include "ConcreteClass.h"

int main(void) {

    ConcreteClass *cc = new ConcreteClass();

    cc->templateMethod();
    return 0;
}