/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
	int a;
	volatile boolean full = false;
	void put(int value) throws InterruptedException {
		synchronized (this) { 
			while(full){
				wait();
			}
			full = true;
			notifyAll();
			a = value;
		}
	}

	int get() throws InterruptedException {
		synchronized (this) {
			while(!full){
				wait();
			} 
			full = false;
			notifyAll();
			return a;
		}
	}
}
