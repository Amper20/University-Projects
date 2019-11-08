package lock;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Semaphore;
/**
 * @author cristian.chilipirea
 *
 *         DO NOT MODIFY
 */
public class Main {
	static final int N = 100000;
	static int N_ITERATIONS = 100;
	static ArrayBlockingQueue<String> a = new ArrayBlockingQueue(100);
	static Semaphore available = new Semaphore(3, true);
	public static void main(String[] args) {
		Thread threads[] = new Thread[4];
		MyLock lock = new MyLock();
		boolean sw = true;
			for (int i = 0; i < 4; i++)
				threads[i] = new Thread(new MyThread(i, lock));
			for (int i = 0; i < 4; i++)
				threads[i].start();
			for (int i = 0; i < 4; i++) {
				try {
					threads[i].join();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
	}
}
