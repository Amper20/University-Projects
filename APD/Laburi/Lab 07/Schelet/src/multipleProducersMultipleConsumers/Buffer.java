
package multipleProducersMultipleConsumers;
import java.util.concurrent.ArrayBlockingQueue;
/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
	ArrayBlockingQueue<Integer> a = new ArrayBlockingQueue(Main.N_CONSUMERS + Main.N_PRODUCERS);

	void put(int value) {
		try {
			a.put(value);
		} catch (InterruptedException e) {
			//TODO: handle exception
			System.out.println(e);
		}
		//a = value;
	}

	int get() {
		try {
			return (int)a.take();
		} catch (InterruptedException e) {
			//TODO: handle exception
		}
		return 0;
	}
}
