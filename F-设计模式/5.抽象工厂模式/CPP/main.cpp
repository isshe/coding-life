#include <iostream>
#include "AbstractFactory.h"
#include "ConcreteFactory1.h"
#include "ConcreteFactory2.h"
#include "AbstractProductA.h"
#include "AbstractProductB.h"

int main()
{
    // 初始化两个工厂
    AbstractFactory *cf1 = new ConcreteFactory1();
    AbstractFactory *cf2 = new ConcreteFactory2();

    // 第一个工厂进行生产
    AbstractProductA *apa = cf1->createProductA();
    delete apa;

    AbstractProductB *apb = cf1->createProductB();
    delete apa;

    // 第二个工厂进行生产
    apa = cf2->createProductA();
    delete apa;

    apb = cf2->createProductB();
    delete apa;

    delete cf1;
    delete cf2;

    return 0;
}