package Java;

// 有硬币状态
public class SoldState implements State {
    private GumballMachine gumballMachine;

    public SoldState(GumballMachine gumballMachine) {
        this.gumballMachine = gumballMachine;
    }

    public void insertQuarter() {
        System.out.println("请等一等你的糖果机...等下再投币...");
    }

    public void ejectQuarter() {
        System.out.println("都转动手柄了，还想退币，想什么呢？");
        //gumballMachine.setState(gumballMachine.getHasQuarterState());
    }
    public void turnCrank() {
        System.out.println("正在转动， 请等一等你的糖果机..");
    }

    public void dispense() {
        System.out.println("正在发放糖果...");
        gumballMachine.releaseBall();
        if (gumballMachine.getCount() > 0) {
            gumballMachine.setState(gumballMachine.getNoQuarterState());
        } else {
            System.out.println("卖完了...");
            gumballMachine.setState(gumballMachine.getSoldOutState());
        }
    }
    public void refill() {
        System.out.println("SoldState::refill()...");
    }

    public String toString() {
        return "发放糖果中...";
    }
}