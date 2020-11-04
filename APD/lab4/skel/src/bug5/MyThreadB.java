package bug5;
/**
 * @author cristian.chilipirea
 * 
 *         Solve the dead-lock
 */

// cat timp thread-ul A modifica valoarea din A, e ok ca thread-ul B sa modifice valoarea din B
// cat value A este in lock de la thread-ul A, value B thebuie sa fie in lock de la thread-ul B. Si viceversa
public class MyThreadB implements Runnable {

	@Override
	public void run() {
		synchronized (Main.lockA) {
			for (int i = 0; i < Main.N; i++)
				Main.valueB++;
			synchronized (Main.lockB) {
				for (int i = 0; i < Main.N; i++)
					Main.valueA++;
			}
		}
	}
}
