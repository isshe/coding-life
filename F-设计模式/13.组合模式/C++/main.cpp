
#include "Component.h"
#include "Composite.h"
#include "Leaf.h"
#include "Client.h"

using namespace std;

int main(void) {

    Composite cSite1;
    Leaf leaf1;
    Leaf leaf2;

    cSite1.add(&leaf1);
    cSite1.add(&leaf2);

    Client client(&cSite1);
    client.printComposite();

    return 0;
}