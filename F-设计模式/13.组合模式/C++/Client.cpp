
#include "Client.h"

//using namespace std;

Client::Client(Component *allComposite) {
    _allComposite = allComposite;
}

void Client::printComposite() {
    _allComposite->print();
}
