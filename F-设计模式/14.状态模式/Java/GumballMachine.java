package Java;

public class GumballMachine {
    State soldOutState;
    State noQuarterState;
    State hasQuarterState;
    State soldState;

    State curState;
    int count = 0;

    public GumballMachine(int numberGumballs) {
        soldOutState = new SoldOutState(this);
        noQuarterState = new NoQuarterState(this);
        hasQuarterState = new HasQuarterState(this);
        soldState = new SoldState(this);

        this.count = numberGumballs;
        if (numberGumballs > 0) {
            curState = noQuarterState;
        } else {
            curState = soldOutState;
        }
    }

    public void insertQuarter() {
        curState.insertQuarter();
    }

    public void ejectQuarter() {
        curState.ejectQuarter();
    }

    public void turnCrank() {
        curState.turnCrank();
        curState.dispense();        // 自动发放糖果，然后转到下一个状态
    }

    public void dispense() {
        curState.dispense();
    }

    public void releaseBall() {
        if (count > 0) {
            count = count - 1;
        }
    }

    public void refill() {
        this.count += count;
        curState.refill();
        System.out.println("new count is: " + this.count);
    }

    public int getCount() {
        return count;
    }

    public void setState(State state) {
        this.curState = state;
    }

    public State getState() {
        return curState;
    }

    public State getSoldOutState() {
        return soldOutState;
    }

    public State getHasQuarterState() {
        return hasQuarterState;
    }

    public State getNoQuarterState() {
        return noQuarterState;
    }

    public State getSoldState() {
        return soldState;
    }

    public String toString() {
        StringBuffer result = new StringBuffer();
        result.append("\nisshe 的糖果机, Inc.");
        result.append("\n库存糖果: " + count + " 粒");
        result.append("\n糖果机 " + curState + "\n");
        return result.toString();
    }
}