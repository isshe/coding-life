package Java;

public class NYPizzaIngredientFactory implements PizzaIngredientFactory {
    public Dough createDough() {
        return new ThinCrustDough();
    }
    public Sauce createSauce() {
        return new MarinaraSauce();
    }
    //public Cheese createCheese();
    //public Veggies[] createVeggies();
    //public Pepperoni createPepperoni();
    //public Clams createClam();
}