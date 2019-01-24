[TOC]

# 单件模式-Java代码示例

## 1. 文件列表
* Java1/：第一个Java示例，对整个`getInstance`方法进行原子限制。
    * 缺点：每次都加锁，性能较只创建时才加锁性能差。
* Java2/：第二个Java示例，静态创建对象（程序一开始就创建）。
    * 不是懒创建，耗资源。
* Java3/：第三个Java示例，对创建对象操作进行原子限制。
    * 前两种方法的综合。

## 2. 各个文件关键代码
* Java1:
```java
public class Singleton {
    private static Singleton _instance;

    private Singleton() {};

    // 这里用原子操作，问题是每次获取都是原子操作，比较慢
    public static synchronized Singleton getInstance() {
        if (_instance == null) {
            _instance = new Singleton();
        }
        return _instance;
    }

    public void saySomething() {
        System.out.println("Singleton...");
    }
}
```

* Java2:
```java
public class Singleton {
    // 开始就建一个对象，缺点：不是懒创建
    private static Singleton _instance = new Singleton();;

    private Singleton() {};

    public static Singleton getInstance() {
        return _instance;
    }

    public void saySomething() {
        System.out.println("Singleton...");
    }
}
```

* Java3:
```java
public class Singleton {
    private volatile static Singleton _instance;

    private Singleton() {};

    public static Singleton getInstance() {
        if (_instance == null) {
            synchronized (Singleton.class) {
                if (_instance == null) {
                    _instance = new Singleton();
                }
            }
        }
        return _instance;
    }

    public void saySomething() {
        System.out.println("Singleton...");
    }
}
```

## x. 疑问

## y. 拓展/总结
* Java3示例中，两次判空应对的场景是：
    * 同时有`2个及以上`线程通过第一个`if (_instance == null)`判断；
    * 第一个线程进入`synchronized`区域，进行对象创建，创建完以后出去。
    * 第二个线程进入`synchronized`区域，如果没有第二个`if (_instance == null)`，就会创建第二个对象。

## z. 参考
* 《Head First设计模式》

