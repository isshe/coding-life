package Java;

public class Test {
    public static void main(String[] args) {
        //MallardDuck duck = new MallardDuck();

        WildTurkey turkey = new WildTurkey();

        TurkeyAdapter adapter = new TurkeyAdapter(turkey);

        adapter.quack();
        adapter.fly();
    }
}