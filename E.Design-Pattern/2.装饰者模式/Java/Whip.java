package Java;

// 具体修饰者类
public class Whip extends CondimentDecorator {
    Beverage beverage;  // 保存被修饰的对象

    public Whip(Beverage beverage) {
        this.beverage = beverage;
    }

    public String getDescription() {
        return beverage.getDescription() + ", Whip";
    }

    public double cost() {
        return beverage.cost() + 0.80;
    }
}