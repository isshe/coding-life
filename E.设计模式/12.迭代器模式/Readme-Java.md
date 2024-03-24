[TOC]

# 迭代器模式-Java 代码示例

## 1. 文件列表
* Iterator.java: Iterator，迭代器抽象类，定义迭代器的接口。
* DinerMenuIterator.java: 具体的迭代器。
* Menu.java: 聚合，集合，抽象类，定义聚合的接口。
* DinerMenu.java: 具体的聚合
* MenuItem.java: 定义聚合里面的元素。
* Waitress.java: client, 使用迭代器和聚合的类。

## 2. 各个文件关键代码
### 2.1 迭代器
* Iterator.java
```java
package Java;

public interface Iterator {
    boolean hasNext();
    MenuItem next();
}
```

### 2.2 具体迭代器
* DinerMenuIterator.java
```java
package Java;

public class DinerMenuIterator implements Iterator {
    MenuItem[] items;
    int position = 0;

    public DinerMenuIterator(MenuItem[] items) {
        this.items = items;
    }

    public MenuItem next() {
        MenuItem menuItem = items[position];
        position = position + 1;
        return menuItem;
    }

    public boolean hasNext() {
        if (position >= items.length || items[position] == null) {
            return false;
        }

        return true;
    }
}
```

### 2.3 聚合
* Menu.java
```java
package Java;

public interface Menu {
    public Iterator createIterator();
}
```

### 2.4 具体聚合
* MenuItem.java: 聚合内元素
```java
package Java;

public class MenuItem {
    String name;
    String desc;
    boolean vegetarian;
    double price;

    public MenuItem(String name, String desc, boolean vegetarian, double price) {
        this.name = name;
        this.desc = desc;
        this.vegetarian = vegetarian;
        this.price = price;
    }

    public String toString() {
        return (name + ", ￥" + price + "\n    " + desc);
    }
}
```

* DinerMenu.java
```java
package Java;

public class DinerMenu implements Menu {
    static final int MAX_ITEMS = 6;
    int numberOfItems = 0;
    MenuItem[] menuItems;

    public DinerMenu() {
        menuItems = new MenuItem[MAX_ITEMS];
        addItem("饭", "白米饭，不吃白不吃", false, 10);
        addItem("鱼", "新鲜鱼，吃了也白吃", false, 1000);
        addItem("香蕉", "水果，--------", false, 900);
    }

    public void addItem(String name, String desc, boolean vegetarian, double price)
    {
        if (numberOfItems >= MAX_ITEMS) {
            System.err.println("Sorry, menu is full!");
            return;
        }

        MenuItem menuItem = new MenuItem(name, desc, vegetarian, price);
        menuItems[numberOfItems] = menuItem;
        numberOfItems = numberOfItems + 1;
    }

    public MenuItem[] getMenuItems() {
        return menuItems;
    }

    public Iterator createIterator() {
        return new DinerMenuIterator(menuItems);
    }

}
```

### 2.5 Client
* Waitress.java
```java
package Java;

public class Waitress {
    Menu dinerMenu;

    public Waitress(Menu dinerMenu) {
        this.dinerMenu = dinerMenu;
    }

    public void printMenu() {
        Iterator dinerIterator = dinerMenu.createIterator();

        System.out.println("--------dinerMenu--------");
        printMenu(dinerIterator);
        System.out.println("------------------------");
    }

    private void printMenu(Iterator iterator) {
        while (iterator.hasNext()) {
            MenuItem menuItem = iterator.next();
            System.out.println(menuItem);
        }
    }
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《Head First 设计模式》

