package Java;

public class Waitress {
    Menu dinerMenu;
    Menu cafeMenu;

    public Waitress(Menu dinerMenu, Menu cafeMenu) {
        this.dinerMenu = dinerMenu;
        this.cafeMenu = cafeMenu;
    }

    public void printMenu() {
        Iterator dinerIterator = dinerMenu.createIterator();
        System.out.println("--------dinerMenu--------");
        printMenu(dinerIterator);
        System.out.println("------------------------");

        Iterator cafeIterator = cafeMenu.createIterator();
        System.out.println("--------cafeMenu--------");
        printMenu(cafeIterator);
        System.out.println("------------------------");
    }

    private void printMenu(Iterator iterator) {
        while (iterator.hasNext()) {
            MenuItem menuItem = iterator.next();
            System.out.println(menuItem);
        }
    }
}