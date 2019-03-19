package Java;

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