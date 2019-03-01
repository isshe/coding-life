package Java;

public class Test {
    public static void main(String[] args) {

        Facade facade = new Facade();
        facade.watchMovie();

        System.out.println("----2 hours gone...");

        facade.endMovie();
    }
}