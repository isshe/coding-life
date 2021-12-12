package Java;

public class Test {
    public static void main(String[] args) {
        NYPizzaStore nps = new NYPizzaStore();
        ChicagoPizzaStore cps = new ChicagoPizzaStore();

        Pizza npz = nps.orderPizza("cheese");
        System.out.println();
        Pizza cpz = cps.orderPizza("cheese");
    }
}