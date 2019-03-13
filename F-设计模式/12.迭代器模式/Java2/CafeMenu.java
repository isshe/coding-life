package Java;

import java.util.*;

public class CafeMenu implements Menu {
    Hashtable<String, MenuItem> menuItems = new Hashtable<String, MenuItem>();

    public CafeMenu() {
        addItem("哈希咖啡", "我乱写的一个名字", true, 39.9);
        addItem("哈希咖啡2", "我乱写的另一个名字", false, 36.9);
        addItem("Burrito", "A large burrito, with whole pinto beans, salsa, guacamole", true , 42.9);

    }

    public void addItem(String name, String desc, boolean vegetarian, double price) {
        MenuItem menuItem = new MenuItem(name, desc, vegetarian, price);
        menuItems.put(menuItem.getName(), menuItem);
    }

    public Hashtable getItems() {
        return menuItems;
    }

    public Iterator createIterator() {
        return new CafeMenuIterator(menuItems);
    }
}