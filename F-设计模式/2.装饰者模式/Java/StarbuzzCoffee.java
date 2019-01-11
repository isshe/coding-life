package Java;

public class StarbuzzCoffee {
    public static void main(String args[]) {
        Beverage beverage = new Espresso();
        System.out.println(beverage.getDescription() + " ￥" + beverage.cost());

        Beverage beverage1 = new HouseBlend();
        beverage1 = new Mocha(beverage1);
        beverage1 = new Mocha(beverage1);
        beverage1 = new Whip(beverage1);
        beverage1 = new Soy(beverage1);
        beverage1 = new Milk(beverage1);
        System.out.println(beverage1.getDescription() + " ￥" + beverage1.cost());

        Beverage beverage2 = new DarkRoast();
        beverage2 = new Mocha(beverage2);
        beverage2 = new Mocha(beverage2);
        beverage2 = new Whip(beverage2);
        beverage2 = new Soy(beverage2);
        beverage2 = new Milk(beverage2);
        System.out.println(beverage2.getDescription() + " ￥" + beverage2.cost());

        Beverage beverage3 = new Decaf();
        beverage3 = new Mocha(beverage3);
        beverage3 = new Whip(beverage3);
        beverage3 = new Soy(beverage3);
        beverage3 = new Milk(beverage3);
        System.out.println(beverage3.getDescription() + " ￥" + beverage3.cost());

    }
}