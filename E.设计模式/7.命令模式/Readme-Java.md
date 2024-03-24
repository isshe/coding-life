[TOC]

# 命令模式-Java 代码示例

## 1. 文件列表
* Command.java: 命令抽象类/接口。
* LightCommand.java: 具体命令类，用于执行`Light`的命令。
* Light.java: Receiver, 实际工作者。
* SimpleRemoteControl.java: Invoker, 触发者类。
* Test.java: 测试主程序类。

## 2. 各个文件关键代码
### 2.1 命令抽象类
```java
public interface Command {
    public void execute();
}
```

### 2.2 具体命令类
```java
public class LightOnCommand implements Command {
    Light light;

    public LightOnCommand(Light light) {
        this.light = light;
    }

    public void execute() {
        light.on();
    }
}
```

### 2.3 接收者类
```java
public class Light {
    public Light() {}

    public void on() {
        System.out.println("Light on...");
    }
}
```

### 2.4 触发者类
```java
public class SimpleRemoteControl {
    Command slot;

    public SimpleRemoteControl() {}

    public void setCommand(Command command) {
        slot = command;
    }

    public void buttonWasPressed() {
        slot.execute();
    }
}
```

### 2.5 测试主程序
```java
public class Test {
    public static void main(String[] args) {
        SimpleRemoteControl remote = new SimpleRemoteControl();
        Light light = new Light();
        LightOnCommand lightOn = new LightOnCommand(light);

        remote.setCommand(lightOn);
        remote.buttonWasPressed();
    }
}
```

## x. 疑问

## y. 拓展/总结

## z. 参考
* 《Head First 设计模式》

