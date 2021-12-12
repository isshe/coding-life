package Java;

public class Test {
    public static void main(String[] args) {
        // 注意用抽象，不用具体类
        PizzaIngredientFactory nypif = new NYPizzaIngredientFactory();
        PizzaIngredientFactory ccgpif = new ChicagoPizzaIngredientFactory();

        System.out.println(nypif.createDough());
        System.out.println(nypif.createSauce());
        System.out.println(ccgpif.createDough());
        System.out.println(ccgpif.createSauce());
    }
}