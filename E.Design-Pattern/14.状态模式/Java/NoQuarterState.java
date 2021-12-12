package Java;

// 有硬币状态
public class NoQuarterState implements State {
    private GumballMachine gumballMachine;

    public NoQuarterState(GumballMachine gumballMachine) {
        this.gumballMachine = gumballMachine;
    }

    public void insertQuarter() {
        System.out.println("你投入了一枚硬币，亲; 现在为您转到有硬币状态...");
        gumballMachine.setState(gumballMachine.getHasQuarterState());
    }

    public void ejectQuarter() {
        System.out.println("你没有投入硬币，亲");
    }
    public void turnCrank() {
        System.out.println("你转动了手柄...但你没有投入硬币，亲");
    }

    public void dispense() {
        System.out.println("你需要先投币");
    }
    public void refill() {
        System.out.println("NoQuarterState::refill()...");
    }

    public String toString() {
        return "等待你投币...";
    }
}