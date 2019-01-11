
package Java;


//Beverage /ˈbev(ə)rij/ => 饮料
// 饮料基类，所有东西基于此
public abstract class Beverage {
    String description = "Unknown Beverage";

    public String getDescription() {
        return description;
    }

    public abstract double cost();
}