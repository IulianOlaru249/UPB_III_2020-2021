package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

public class Buffer {
	int value;
	ArrayBlockingQueue queue;

	public Buffer() {
		queue = new ArrayBlockingQueue(100);
	}

	void put(int value) {
		try {
			queue.put(value);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	int get() {
		try {
			value = (int)queue.take();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return value;
	}
}
