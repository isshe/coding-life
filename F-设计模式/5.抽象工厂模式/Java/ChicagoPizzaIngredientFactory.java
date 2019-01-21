package Java;

public class ChicagoPizzaIngredientFactory implements PizzaIngredientFactory {
    public Dough createDough() {
        return new ThickCrustDough();
    }
    public Sauce createSauce() {
        return new PlumTomatoSauce();
    }
    //public Cheese createCheese();
    //public Veggies[] createVeggies();
    //public Pepperoni createPepperoni();
    //public Clams createClam();
}