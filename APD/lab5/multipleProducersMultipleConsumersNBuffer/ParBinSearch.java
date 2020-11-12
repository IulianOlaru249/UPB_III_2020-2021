package multipleProducersMultipleConsumersNBuffer;

import oneProducerOneConsumer.Main;

import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.List;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class ParBinSearch {

    static int P = 4;
    static List<Integer> arr = new ArrayList<>();
    static int el = 9;
    public static CyclicBarrier barrier;
    static int arrayStart = 0;
    static int arrayEnd;
    static int index = -1;

    public static void main(String[] args) throws InterruptedException, ConcurrentModificationException {
        arr.add(1);
        arr.add(2);
        arr.add(3);
        arr.add(4);
        arr.add(5);
        arr.add(6);
        arr.add(7);
        arr.add(8);
        arr.add(9);
        arr.add(10);
        arrayEnd = arr.size() - 1;
        barrier = new CyclicBarrier(P);

        ArrayList<MyThread> threads = new ArrayList<>();

        for (int i = 0; i < P; i++) {
            MyThread t = new MyThread(i);
            threads.add(t);
        }
        for (MyThread t : threads) {
            t.start();
        }
        for (MyThread t : threads) {
            try {
                t.join();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        System.out.println(ParBinSearch.index);
    }
}
