package Java;

public class CheesePizza implements Pizza {
    public void prepare() {
        System.out.println("CheesePizza prepare");
    }

    public void bake(){
        System.out.println("CheesePizza bake");
    }

    public void cut(){
        System.out.println("CheesePizza cut");
    }

    public void box(){
        System.out.println("CheesePizza box");
    }
}