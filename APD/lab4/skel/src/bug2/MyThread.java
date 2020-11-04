package bug2;
/**
 * @author cristian.chilipirea
 * 
 *         Why does this code not block? We took the same lock twice!
 */
public class MyThread implements Runnable {
	static int i;

	//poti avea un thread care poate face lock de doua ori la rand;
	// in java poti ptc o metoda syncronized poate apela alta metoda syncronized

	@Override
	public void run() {
		synchronized (this) {
			synchronized (this) {
				i++;
			}
		}
	}
}
