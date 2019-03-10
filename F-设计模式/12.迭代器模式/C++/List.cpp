
#include "List.h"

template <class Item>
List<Item>::List(long size) {
    _size = size;
    _count = 0;
    _items = new Item[size];
}


template <class Item>
long List<Item>::Count() const {
    return _count;
}


template <class Item>
long List<Item>::Size() const {
    return _size;
}


template <class Item>
Item& List<Item>::Get(long index) const {
    if (index >= Count()) {
        throw -1;
    }

    return _items[index];
}


template <class Item>
Item& List<Item>::First() const {
    if (_count <= 0) {
        throw -1;
    }

    return _items[0];
}

template <class Item>
Item& List<Item>::Last() const {
    if (_count >= _size) {
        throw -1;
    }

    return _items[_count - 1];
}

template <class Item>
void List<Item>::Append(const Item& item) {
    if(_count < _size) {
        _items[_count++] = item;
    }
}

template <class Item>
void List<Item>::RemoveLast() {
    _count--;
}

