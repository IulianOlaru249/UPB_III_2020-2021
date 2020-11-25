package com.company;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.concurrent.BrokenBarrierException;

public class MyThread implements Runnable {
    int id;
    int N;
    int L = 4;
    int cores;

    MyThread(int id, int N, int cores ){
        this.id = id;
        this.N = L * L + 1;
        this.cores = cores;
    }

    @Override
    public void run() {
        int start = id * L / cores;
        int end = Math.min((id + 1) * L / cores, L);
        int aux[] = new int[L];
        System.out.println("Thread: " + this.id);

        for (int k = 0; k < Math.log(N) + 1; k++) {
            // se sorteaza liniile pare crescator
            // se sorteaza liniile impare descrescator
            for (int i = start; i < end; i++) {
                if (i % 2 != 0) {
                    Arrays.sort(Main.M[i], Collections.reverseOrder());
                } else {
                    Arrays.sort(Main.M[i]);
                }
            }

           try {
                Main.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

            // se sorteaza coloanele descrescator
            for (int i = start; i < end; i++) {
                for (int j = 0; j < L; j++) {
                    aux[j] = Main.M[j][i];
                }

                Arrays.sort(aux);

                for (int j = 0; j < L; j++) {
                    Main.M[j][i] = aux[j];
                }
            }

            try {
                Main.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}


