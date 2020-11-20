package synchronizedSortedList;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Main {
    private final static int N_ITERATIONS = 100;
    private final static int NUMBER_OF_THREADS = 4;
    public static Semaphore sem;

    public static void main(String[] args) {
        Thread[] threads = new Thread[4];
        boolean sw = true;

        for (int i = 0; i < N_ITERATIONS; i++) {
            sem = new Semaphore(-2);
            List<Integer> list = Collections.synchronizedList (new ArrayList<Integer>());

            threads[0] = new Reader("/home/ghostpants/Documents/facultateAn3/APD/lab06/elemente1.txt", list);
            threads[1] = new Reader("/home/ghostpants/Documents/facultateAn3/APD/lab06/elemente2.txt", list);
            threads[2] = new Reader("/home/ghostpants/Documents/facultateAn3/APD/lab06/elemente3.txt", list);
            threads[3] = new Sort(list);

            for (int j = 0; j < NUMBER_OF_THREADS; j++) {
                threads[j].start();
            }

            for (int j = 0; j < NUMBER_OF_THREADS; j++) {
                try {
                    threads[j].join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            for (int j = 0; j < list.size() - 1; j++) {
                if (list.get(j) > list.get(j + 1)) {
                    sw = false;
                    System.out.println("Incorrect result: " + list.get(j) + " > " +  list.get(j + 1));
                    break;
                }
            }
        }

        if (sw) {
            System.out.println("Correct");
        }
    }
}
