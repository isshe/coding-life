
#include "Adaptee.h"
#include "Adapter.h"
#include "Target.h"

int main(void) {

    Adaptee *adaptee = new Adaptee();

    Adapter *adapter = new Adapter(adaptee);

    // 实际调用的是adaptee的specificRequest()
    adapter->request();

    return 0;
}