package Java;

// 具体修饰者类
public class Soy extends CondimentDecorator {
    Beverage beverage;  // 保存被修饰的对象

    public Soy(Beverage beverage) {
        this.beverage = beverage;
    }

    public String getDescription() {
        return beverage.getDescription() + ", Soy";
    }

    public double cost() {
        return beverage.cost() + 0.60;
    }
}