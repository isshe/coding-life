package Java;

public class NYPizzaStore extends PizzaStore {
    Pizza createPizza(String type) {

        if (type.equals("cheese")) {
            return new NYStyleCheesePizza();
        } else if (type.equals("peperoni")) {
            //return new NYStylePeperoniePizza();
        } else if (type.equals("clam")) {
            //return new NYStyleClamPizza();
        } else if (type.equals("veggie")) {
            //return new NYStyleVeggiePizza();
        }

        return null;
    }
}