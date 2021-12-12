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