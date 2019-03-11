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