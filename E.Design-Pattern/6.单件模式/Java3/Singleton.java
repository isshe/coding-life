package Java;

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