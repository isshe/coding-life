[TOC]

# 状态模式-Java示例
状态转换：
```java
NoQuarterState->HasQuarterState->SoldState->SoldOutState
(没币)->(有币)->(出售)->(售完)
```

## 1. 文件列表
* GumballMachine.java: Context, 保存state，设置state
* State.java: State，抽象类，实现缺省实现。
* HasQuarterState.java: ConcreteState，具体状态，继承自State。
* NoQuarterState.java: ConcreteState，具体状态，继承自State。
* SoldState.java: ConcreteState，具体状态，继承自State。
* SoldOutState.java: ConcreteState，具体状态，继承自State。
* Test.java：测试类。


## 2. 各个文件关键代码
### 2.1 Context
* GumballMachine.java
```java
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
```

### 2.2 State
* State.java
```java
public interface State {
    public void insertQuarter();
    public void ejectQuarter();
    public void turnCrank();
    public void dispense();
    public void refill();
}
```

### 2.3 ConcreteState
* HasQuarterState.java: ConcreteState，具体状态，继承自State。
```java
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
```

* NoQuarterState.java: ConcreteState，具体状态，继承自State。
```java
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
```

* SoldState.java: ConcreteState，具体状态，继承自State。
```java
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
```

* SoldOutState.java: ConcreteState，具体状态，继承自State。
```java
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
```

### 2.4 测试
* Test.java
```java
public class Test {
    public static void main(String[] args) {
        GumballMachine gumballMachine = new GumballMachine(1);  // NoQuarter状态

        System.out.println(gumballMachine);

        gumballMachine.insertQuarter(); // HasQuarter状态
        gumballMachine.turnCrank(); // Sold状态
        //gumballMachine.dispense();
        System.out.println(gumballMachine);
    }
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《Head First 设计模式》

