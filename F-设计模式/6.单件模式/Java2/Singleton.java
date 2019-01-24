package Java;

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