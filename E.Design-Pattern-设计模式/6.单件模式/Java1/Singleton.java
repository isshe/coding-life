package Java;

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