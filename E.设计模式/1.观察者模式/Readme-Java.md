[TOC]

# 观察者模式-Java 实现示例
* 此示例使用`推`的方式。
* 示例《气象监测应用》描述：
    * 气象站 (Weather Station) 提供气象数据（温度、湿度、气压）；
    * 实现一个应用，气象站提供数据后，更新到对应的显示板上；
    * 显示板支持自定义：其他开发人员可以写自己的显示板；

## 1. 文件列表
* Subject.java: 主题，实现为`interface`。
* Observer.java：观察者，实现为`interface`。
* DisplayElement.java: 显示，实现为`interface`，供实现`具体观察者`使用，每个观察者可以有不同的`display()`。
* WeatherData.java: 主题的一个实现——具体主题。
* CurrentConditionsDisplay.java: 观察者的一个实现——具体观察者。
* WeatherStation.java: 入口，测试程序。


## 2. 主题
* 实现为`interface`;
* 提供`注册(registerObserver)`、`移除(removeObserver)`、`通知(notifyObservers)`接口;
* 实现此主题的具体主题，要实现这些必要的接口。
```java
public interface Subject {
    public void registerObserver(Observer o);
    public void removeObserver(Observer o);
    public void notifyObservers();
}
```

## 3. 观察者
* 实现为`interface`，具体的观察者需要实现`update()`接口。
```java
public interface Observer {
    public void update(float temperature, float humidity, float pressure);
}
```

## 4. 显示板
* 实现为`interface`，提供`display()`接口。
```java
public interface DisplayElement {
    public void display();
}
```
* 感觉这部分对于观察者模式来说，有些多余，起到一定的干扰作用。

## 5. 具体的主题
* 构造函数：初始化一个数组，用于存储此主题下的所有观察者。
* 注册：观察者调用此函数进行注册，以得到通知。
* 移除：观察者调用此函数，将自己从此主题中移除，不再接收通知。
* 通知：主题通知此主题下的所有观察者。
```java
import java.util.*;
public class WeatherData implements Subject {
    private ArrayList<Observer> observers;          // 保存此主题下的所有观察者。
    private float temperature;                      // 保存一些需要的数据
    private float humidity;
    private float pressure;

    public WeatherData() {
        observers = new ArrayList<Observer>();
    }

    public void registerObserver(Observer o) {
        observers.add(o);
    }

    public void removeObserver(Observer o) {
        int i = observers.indexOf(o);
        if (i >= 0) {
            observers.remove(i);
        }
    }

    public void notifyObservers() {
        for (Observer observer : observers) {
            observer.update(temperature, humidity, pressure);
        }
    }

    // measurementsChanged 函数是为了：确定数据变化时，才调用 notifyObservers()（而不是每次都直接调用）
    // 这里实现做了简化，实际应用时，这样更好
    public void measurementsChanged() {
        notifyObservers();
    }

    public void setMeasurements(float temperature, float humidity, float pressure) {
        this.temperature = temperature;
        this.humidity = humidity;
        this.pressure = pressure;
        measurementsChanged();
    }
}
```

## 6. 具体的观察者
* 此类实现`Observer`和`DisplayElement`接口。
    * `Observer`使类能成为观察者。
    * `DisplayElement`提供显示方法。
* 此类当前没有取消注册，个人理解应该是在`析构`函数中调用`weatherData.removeObserver`
```java
public class CurrentConditionsDisplay implements Observer, DisplayElement {
    private float temperature;
    private float humidity;
    private Subject weatherData; // 这里保存主题对象，因为取消注册时，还要用

    public CurrentConditionsDisplay(Subject weatherData) {
        this.weatherData = weatherData;
        weatherData.registerObserver(this);
    }

    public void update(float temperature, float humidity, float pressure) {
        this.temperature = temperature;
        this.humidity = humidity;
        display();
    }

    public void display() {
        System.out.println("Current conditions: " + temperature
                + "F degrees and " + humidity + "% humidity");
    }
}

```

## 7. 气象站——测试程序
```java
public class WeatherStation {
    public static void main(String[] args) {
        WeatherData wd = new WeatherData();                             // 具体的主题
        CurrentConditionsDisplay cd = new CurrentConditionsDisplay(wd); // 具体的观察者

        wd.setMeasurements(80, 65, 30.4f);
        wd.setMeasurements(82, 70, 29.2f);
        wd.setMeasurements(78, 90, 29.4f);
    }
}
```


## x. 参考
* 《Head First 设计模式》




