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