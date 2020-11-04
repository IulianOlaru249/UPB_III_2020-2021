package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class MyThread implements Runnable {
    int id;
    int N;
    int cores;

    MyThread(int id, int N, int cores ){
        this.id = id;
        this.N = N;
        this.cores = cores;
    }

    @Override
    public void run() {
        int start = id * N / cores;
        int end = Math.min((id + 1) * N / cores, N);
        for (int k = 0; k < N; k++) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < 5; j++) {
                    Main.graph[i][j] = Math.min(Main.graph[i][k] + Main.graph[k][j], Main.graph[i][j]);
                }
            }
            try {
                Main.barrier.await();
            }
            catch (BrokenBarrierException | InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
