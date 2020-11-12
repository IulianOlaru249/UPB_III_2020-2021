package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;

/**
 * @author Gabriel Gutu <gabriel.gutu at upb.ro>
 *
 */
public class Buffer {
    
    Queue queue;
    
    public Buffer(int size) {
        queue = new LimitedQueue(size);
    }

    synchronized void put(int value) {
            while (!this.queue.isEmpty()) {
                try {this.wait();}catch(Exception ex) {}
            }

        queue.add(value);
        notifyAll();
	}

    synchronized int get() {
        while (this.queue.isEmpty()) {
            try {this.wait();}catch(Exception ex) {}
        }
        this.notifyAll();
        return (int)queue.poll();
	}
}
