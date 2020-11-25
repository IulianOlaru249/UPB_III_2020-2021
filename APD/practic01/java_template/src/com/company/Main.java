package com.company;


import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class Main {

    static int L;
    static int P;
    static Integer[][] M = {
        {9, 6, 9, 4},
        {2, 7, 6, 5},
        {9, 3, 6, 2},
        {5, 4, 1, 5},
    };

    static Integer[][] M2 = {
            {9, 6, 9, 4},
            {2, 7, 6, 5},
            {9, 3, 6, 2},
            {5, 4, 1, 5},
    };
    static CyclicBarrier barrier;
    //static int N = 100000013;
    //static int v[] = new int[N];

    public static void main(String[] args) {
        long startTime = System.nanoTime();
        //Scanner.systemIn();
        Scanner console = new Scanner(System.in);
        L = console.nextInt();
        P = console.nextInt();

        L = 4;
        //M = new Integer[L][L];

        //int cores = Runtime.getRuntime().availableProcessors();
        int cores = P;
        barrier = new CyclicBarrier(cores);
        Thread threads[] = new Thread[cores];


        for (int i = 0; i < cores; i++)
            threads[i] = new Thread(new MyThread(i, L, cores));

        for (int i = 0; i < cores; i++)
            threads[i].start();

        for (int i = 0; i < cores; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        long stopTime = System.nanoTime();
        System.out.println("Paralel: " + (stopTime - startTime));

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                System.out.print(M[i][j] + " ");
            System.out.println();
        }



        /* Serial*/
        int N = L * L;
        startTime = System.nanoTime();
        for (int k = 0; k < Math.log(N) + 1; k++) {
            // se sorteaza liniile pare crescator
            // se sorteaza liniile impare descrescator
            for (int i = 0; i < L; i++) {
                if (i % 2 != 0) {
                    Arrays.sort(Main.M2[i], Collections.reverseOrder());
                } else {
                    Arrays.sort(Main.M2[i]);
                }
            }

            int aux[] = new int[L];
            // se sorteaza coloanele descrescator
            for (int i = 0; i < L; i++) {
                for (int j = 0; j < L; j++) {
                    aux[j] = Main.M2[j][i];
                }
                //Arrays.sort(Main.M2[i]);
                Arrays.sort(aux);

                for (int j = 0; j < L; j++) {
                    Main.M2[j][i] = aux[j];
                }
            }
        }

        System.out.println();
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < L; j++)
                System.out.print(M2[i][j] + " ");
            System.out.println();
        }
        stopTime = System.nanoTime();
        System.out.println("Serial: " + (stopTime - startTime));
    }

}
