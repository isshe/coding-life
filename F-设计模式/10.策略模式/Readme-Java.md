[TOC]

# 策略模式-Java代码示例

## 1. 文件列表
* Duck.java: 对应类图中Context（抽象类）
* MallardDuck.java: 一个具体的Duck，具体的Context
* FlyBehavior.java: 对应类图中Strategy（抽象类、接口）
* FlyNoWay.java:  对应类图中ConcreteStrategy, FlyBehavior的一个子类
* FlyWithWings.java:  对应类图中ConcreteStrategy, FlyBehavior的一个子类
* QuackBehavior.java: 对应图中的Strategy（抽象类，接口）
* Quack.java: 对应类图中ConcreteStrategy, QuackBehavior的一个子类
* Squack.java: 对应类图中ConcreteStrategy, QuackBehavior的一个子类
* MuteQuack.java: 对应类图中ConcreteStrategy, QuackBehavior的一个子类

## 2. 各个文件关键代码
### 2.1 Context
* Duck.java
```java
public abstract class Duck {
    FlyBehavior flyBehavior;
    QuackBehavior quackBehavior;
    public Duck() {

    }

    public abstract void display();

    public void performFly() {
        flyBehavior.fly();
    }

    public void performQuack() {
        quackBehavior.quack();
    }

    public void swim() {
        System.out.println("All ducks float, even decoys!");
    }
}
```

* MallardDuck.java
```java
public class MallardDuck extends Duck {
    public MallardDuck() {
        quackBehavior = new Quack();
        flyBehavior = new FlyWithWings();
    }

    public void display() {
        System.out.println("i'm real Mallard duck");
    }
}
```

### 2.2 StrategyA & ConcreteStrategyAx
* FlyBehavior.java
```java
public interface FlyBehavior {
    public void fly();
}
```


* FlyNoWay.java
```java
public class FlyNoWay implements FlyBehavior {
    public void fly() {
        System.out.println("I can't fly");
    }
}
```

* FlyWithWings.java
```java
public class FlyWithWings implements FlyBehavior {
    public void fly() {
        System.out.println("I'm flying");
    }
}
```

### 2.3 StrategyB & ConcreteStrategyBx
* QuackBehavior.java
```java
public interface QuackBehavior {
    public void quack();
}
```

* Quack.java
```java
public class Quack implements QuackBehavior {
    public void quack() {
        System.out.println("Quack");
    }
}
```

* MuteQuack.java
```java
public class MuteQuack implements QuackBehavior {
    public void quack() {
        System.out.println("<< Silence >>");
    }
}
```

* Squack.java
```java
public class Squack implements QuackBehavior {
    public void quack() {
        System.out.println("Squack");
    }
}
```

### 2.4 Test
* Test.java
```java
public class Test {
    public static void main(String[] args) {
        Duck mallard = new MallardDuck();
        mallard.performQuack();
        mallard.performFly();
    }
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《Head First 设计模式》

