package Java;

// 有硬币状态
public class SoldOutState implements State {
    private GumballMachine gumballMachine;

    public SoldOutState(GumballMachine gumballMachine) {
        this.gumballMachine = gumballMachine;
    }

    public void insertQuarter() {
        System.out.println("没有糖果了，不能投币...");
    }

    public void ejectQuarter() {
        System.out.println("没有投币，不能退币...");
    }
    public void turnCrank() {
        System.out.println("没有投币，不能转动...");
    }

    public void dispense() {
        System.out.println("糖果卖完，不能发放...");
    }
    public void refill() {
        System.out.println("填充糖果中...");
        gumballMachine.setState(gumballMachine.getNoQuarterState());
        System.out.println("填充完毕...");
    }

    public String toString() {
        return "售罄...";
    }
}