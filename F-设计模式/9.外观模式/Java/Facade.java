package Java;

// 这个示例没有实现为interface/抽象类。
public class Facade {
    private Dvd dvd;
    private Lights lights;
    private Screen screen;
    public Facade() {
        dvd = new Dvd();
        lights = new Lights();
        screen = new Screen();
    }

    public void watchMovie() {
        dvd.on();
        lights.off();
        screen.down();
    }

    public void endMovie() {
        dvd.off();
        lights.on();
        screen.up();
    }
}