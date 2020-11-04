package doubleVectorElements;

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
        for (int i = start; i < end; i++)
            Main.v[i] = Main.v[i] * 2;
    }
}
