#ifndef _LIST_H_
#define _LIST_H_

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

private:
    Item *_items; // 数组
    long _count;
    long _size;
};

#endif