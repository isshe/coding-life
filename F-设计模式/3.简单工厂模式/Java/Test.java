package Java;

public class Test {
    public static void main(String[] arga) {
        SimplePizzaFactory factory = new SimplePizzaFactory();
        PizzaStore store = new PizzaStore(factory);

        Pizza pizza = store.orderPizza("cheese");
        System.out.println("");

        pizza = store.orderPizza("peperoni");
        System.out.println("");

        pizza = store.orderPizza("clam");
        System.out.println("");

        pizza = store.orderPizza("veggie");
        System.out.println("");

    }
}