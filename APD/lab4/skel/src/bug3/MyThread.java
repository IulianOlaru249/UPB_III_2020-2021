package bug3;
/**
 * @author cristian.chilipirea
 * 
 *         Why is value set correct even though we use different locks in
 *         different threads?
 */
public class MyThread implements Runnable {
	//daca fac lock pe obiecte diferite ar trebui sa pot rula in paralel deci sa am race condition
	//ideea e ca se face sincronizarea ptc
	static String a = "LOCK";
	static String b = "LOCK";
	int id;
	static int value = 0;

	MyThread(int id) {
		this.id = id;
	}

	@Override
	public void run() {
		if (id == 0) {
			synchronized (this) {
				for (int i = 0; i < Main.N; i++)
					value = value + 3;
			}
		} else {
			synchronized (this) {
				for (int i = 0; i < Main.N; i++)
					value = value + 3;
			}
		}
	}
}
