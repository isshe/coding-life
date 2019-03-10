[TOC]

# 迭代器模式-C++代码示例

## 1. 文件列表
* Iterator.h: 迭代器抽象类，定义迭代器的接口。
* ListIterator.h/cpp: 具体迭代器。
* List.h/cpp: ConcreteAggregate，具体聚合，链表。
* Student.h/cpp: 存放在链表中的对象。
* main.cpp: 入口函数，测试程序。

## 2. 各个文件关键代码
### 2.1 Iterator
* Iterator.h
```cpp
template <class Item>
class Iterator {
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual Item CurrentItem() const = 0;
protected:
    Iterator();
};
```

### 2.2 ConcreteIterator
* ListIterator.h
```cpp
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
```

* ListIterator.cpp
```cpp
#include "ListIterator.h"

template <class Item>
ListIterator<Item>::ListIterator(const List<Item>* aList)
{
    _list = aList;
    _current = 0;
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

    return _list->Get(_current);
}
```

### 2.3 ConcreteAggregate
* List.h
```cpp
#define DEFAULT_LIST_CAPACITY 128

template <class Item>
class List {
public:
    List(long size = DEFAULT_LIST_CAPACITY);

    long Count() const;
    long Size() const;
    Item& Get(long index) const;
    Item& First() const;
    Item& Last() const;
    void Append(const Item&);
    void RemoveLast();

    // createListIterator();

private:
    Item *_items; // 数组
    long _count;
    long _size;
};
```

* List.cpp
```cpp

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
```

### 2.4 聚合内元素
* Student.h
```cpp
class Student {
public:
    Student(long id);
    void print();

private:
    long _id;
};
```

* Student.cpp
```cpp

#include "Student.h"
#include <iostream>

Student::Student(long id) {
    _id = id;
}

void Student::print() {
    std::cout << "---Student::Say()---" << _id << "---" << std::endl;
}
```


### 2.5 Test
* main.cpp
```cpp

#include "List.h"
#include "ListIterator.h"
#include "Iterator.h"
#include "Student.h"

int main(void) {

    Student std1 = Student(1);
    Student std2 = Student(2);
    Student std3 = Student(3);

    List<Student> list(10);

    list.Append(std1);
    list.Append(std2);
    list.Append(std3);

    ListIterator<Student> stdIter(&list);

    for (stdIter.First(); !stdIter.IsDone(); stdIter.Next()) {
        stdIter.CurrentItem().print();
    }

    return 0;
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《设计模式 可复用面向对象软件的基础》

