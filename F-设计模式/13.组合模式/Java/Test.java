package Java;

public class Test {
    public static void main(String[] args) {
        MenuComponent allMenu = new Menu("All Menu", "All menus combined");
        MenuComponent dinerMenu = new Menu("Diner Menu", "Lunch");

        MenuItem BLT = new MenuItem("BLT", "xxxxx BLT", false, 29.9);
        MenuItem vBLT = new MenuItem("BLT", "xxxxx vBLT", true, 30.9);

        dinerMenu.add(BLT);
        dinerMenu.add(vBLT);

        allMenu.add(dinerMenu);

        Waitress waitress = new Waitress(allMenu);
        waitress.printMenu();
    }
}