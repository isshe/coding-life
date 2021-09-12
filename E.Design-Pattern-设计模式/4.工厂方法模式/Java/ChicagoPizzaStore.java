package Java;

public class ChicagoPizzaStore extends PizzaStore {
    Pizza createPizza(String type) {

        if (type.equals("cheese")) {
            return new ChicagoStyleCheesePizza();
        } else if (type.equals("peperoni")) {
            //return new ChicagoStylePeperoniePizza();
        } else if (type.equals("clam")) {
            //return new ChicagoStyleClamPizza();
        } else if (type.equals("veggie")) {
            //return new ChicagoStyleVeggiePizza();
        }

        return null;
    }
}