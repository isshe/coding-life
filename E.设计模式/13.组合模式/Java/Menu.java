package Java;

import java.util.Iterator;
import java.util.ArrayList;

public class Menu extends MenuComponent {
    // 菜单项
    ArrayList<MenuComponent> menuComponents = new ArrayList<MenuComponent>();
    String name;
    String desc;

    public Menu(String name, String desc) {
        this.name = name;
        this.desc = desc;
    }

    public void add(MenuComponent menuComponent) {
        menuComponents.add(menuComponent);
    }

    public void remove(MenuComponent menuComponent) {
        menuComponents.remove(menuComponent);
    }

    public MenuComponent getChild(int i) {
        return menuComponents.get(i);
    }

    public String getName() {
        return name;
    }

    public String getDescription() {
        return desc;
    }

    public void print() {
        System.out.print("\n" + getName());
        System.out.println(", " + getDescription());
        System.out.println("---------------------");

        Iterator<MenuComponent> iterator = menuComponents.iterator();
        while(iterator.hasNext()) {
            MenuComponent menuComponent = iterator.next();
            menuComponent.print();
        }
    }
}
