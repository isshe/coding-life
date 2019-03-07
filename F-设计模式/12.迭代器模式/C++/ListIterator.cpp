#ifndef _LIST_ITERATOR_H_
#define _LIST_ITERATOR_H_

#include "ListIterator.h"

template <class Item>
ListIterator<Item>::ListIterator(const List<Item>* aList) : _list(aList), _current(0)
{
}

template <class Item>
void ListIterator<Item>::First() {
    _current = 0;
}

template <class Item>
void ListIterator<Item>::Next() {
    _current++;
}

template <class Item>
bool ListIterator<Item>::IsDone() const {
    return _current >= _list->Count();
}

template <class Item>
Item ListIterator<Item>::CurrentItem() const {
    if (IsDone()) {
        throw -1;
    }

    return _list->Get(_current)
}

#endif