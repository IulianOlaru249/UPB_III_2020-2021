package task1;
import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable implements Runnable {
	static int[][] graph;
	ExecutorService tpe;
	AtomicInteger inQueue;

	public MyRunnable(int[][] graph, ExecutorService tpe, AtomicInteger inQueue) {
		graph = this.graph;
		this.tpe = tpe;
		this.inQueue = inQueue;
	}

	@Override
	public void run() {
		int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        for (int[] ints : graph) {
            if (ints[0] == lastNodeInPath) {
                if (partialPath.contains(ints[1]))
                    continue;
                ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);
                //getPath(newPartialPath, destination);
            }
        }
	}
}
