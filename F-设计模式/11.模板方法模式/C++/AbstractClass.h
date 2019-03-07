#ifndef _ABSTRACTCLASS_H_
#define _ABSTRACTCLASS_H_

class AbstractClass {
public:
    void templateMethod();
    virtual void primitiveOperation1() = 0;
    virtual void primitiveOperation2() = 0;
};


#endif