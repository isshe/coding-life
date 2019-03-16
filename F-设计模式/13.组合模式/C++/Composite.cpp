
#include "Composite.h"

//using namespace std;

void Composite::add(Component *component) {
    _items.push_back(component);
}

void Composite::print() {
    cout << "composite::print()" << endl;
    for(int i = 0; i < _items.size(); i++) {
        _items[i]->print();
    }
}
