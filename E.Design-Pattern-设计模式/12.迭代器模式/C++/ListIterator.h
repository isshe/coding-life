#ifndef _LIST_ITERATOR_H_
#define _LIST_ITERATOR_H_

#include "Iterator.h"
#include "List.h"

template <class Item>
class ListIterator : public Iterator<Item> {
public:
    ListIterator(const List<Item>* aList);
    void First();
    void Next();
    bool IsDone() const;
    Item CurrentItem() const;
private:
    const List<Item>* _list;
    long _current;
};

#endif