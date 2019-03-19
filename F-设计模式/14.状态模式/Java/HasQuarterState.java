package Java;

// 有硬币状态
public class HasQuarterState implements State {
    private GumballMachine gumballMachine;

    public HasQuarterState(GumballMachine gumballMachine) {
        this.gumballMachine = gumballMachine;
    }

    public void insertQuarter() {
        System.out.println("已经投入硬币，不能再投入了，土豪");
    }

    public void ejectQuarter() {
        System.out.println("已退出硬币！");
        gumballMachine.setState(gumballMachine.getNoQuarterState());
    }
    public void turnCrank() {
        System.out.println("你转动了手柄...状态转为：正在出售状态...");
        gumballMachine.setState(gumballMachine.getSoldState());
    }

    public void dispense() {
        System.out.println("HasQuarterState::dispense()...");
    }
    public void refill() {
        System.out.println("HasQuarterState::refill()...");
    }

    public String toString() {
        return "等待你转动手柄...";
    }
}