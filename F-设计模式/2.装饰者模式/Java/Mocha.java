package Java;

// 具体修饰者类
public class Mocha extends CondimentDecorator {
    Beverage beverage;  // 保存被修饰的对象

    public Mocha(Beverage beverage) {
        this.beverage = beverage;
    }

    public String getDescription() {
        return beverage.getDescription() + ", Mocha";
    }

    public double cost() {
        return beverage.cost() + 0.20;
    }
}