package shortestPathsFloyd_Warshall;

import java.util.concurrent.CyclicBarrier;

/**
 * @author cristian.chilipirea
 *
 */
public class Main {
	static int M = 9;
	static int graph[][] = { { 0, 1, M, M, M },
			{ 1, 0, 1, M, M },
			{ M, 1, 0, 1, 1 },
			{ M, M, 1, 0, M },
			{ M, M, 1, M, 0 } };

	static CyclicBarrier barrier;

	public static void main(String[] args) {
		int graph_seq[][] = { { 0, 1, M, M, M },
				          { 1, 0, 1, M, M }, 
				          { M, 1, 0, 1, 1 }, 
				          { M, M, 1, 0, M },
				          { M, M, 1, M, 0 } };


		int cores = Runtime.getRuntime().availableProcessors();
		Thread threads[] = new Thread[cores];
		barrier = new CyclicBarrier(cores);

		for (int i = 0; i < cores; i++)
			threads[i] = new Thread(new MyThread(i, 5, cores));

		for (int i = 0; i < cores; i++)
			threads[i].start();

		for (int i = 0; i < 2; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}


		System.out.println("Paralel:");
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph[i][j] + " ");
			}
			System.out.println();
		}
		System.out.println();




		// Parallelize me (You might want to keep the original code in order to compare)
		for (int k = 0; k < 5; k++) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					graph_seq[i][j] = Math.min(graph_seq[i][k] + graph_seq[k][j], graph_seq[i][j]);
				}
			}
		}


		System.out.println("Sequential:");
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph_seq[i][j] + " ");
			}
			System.out.println();
		}
	}
}
