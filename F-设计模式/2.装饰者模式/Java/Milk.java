package Java;

// 具体修饰者类
public class Milk extends CondimentDecorator {
    Beverage beverage;  // 保存被修饰的对象

    public Milk(Beverage beverage) {
        this.beverage = beverage;
    }

    public String getDescription() {
        return beverage.getDescription() + ", Milk";
    }

    public double cost() {
        return beverage.cost() + 0.40;
    }
}