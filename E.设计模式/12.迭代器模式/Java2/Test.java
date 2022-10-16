package Java;

public class Test {
    public static void main(String[] args) {
        DinerMenu dinerMenu = new DinerMenu();
        CafeMenu cafeMenu = new CafeMenu();
        Waitress waitress = new Waitress(dinerMenu, cafeMenu);
        waitress.printMenu();
    }
}