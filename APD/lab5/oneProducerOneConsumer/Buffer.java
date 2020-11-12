package oneProducerOneConsumer;
/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
	int a;
	boolean empty;

	Buffer () {
		this.empty = true;
	}

	synchronized void put(int value) { //Produce
		while (!this.empty) {
			try {this.wait();}catch(Exception ex) {}
		}

		a = value;
		this.empty = false;
		this.notifyAll();
	}

	synchronized int get() {	//Consume
		while (this.empty) {
			try {this.wait();}catch(Exception ex) {}
		}

		this.notifyAll();
		this.empty = true;
		return a;
	}
}
