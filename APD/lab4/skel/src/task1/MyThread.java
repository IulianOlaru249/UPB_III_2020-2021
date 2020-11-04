package task1;
/**
 * @author cristian.chilipirea
 *
 *         Why is value set correct even though we do not use
 *         locks/synchronized?
 *         DO NOT MODIFY
 */
public class MyThread implements Runnable {
    int id;

    MyThread(int id) {
        this.id = id;
    }

    @Override
    public void run() {
        System.out.println("Hello from thread " + this.id);
    }
}