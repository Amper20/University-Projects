
/**
 * @author cristian.chilipirea
 * 
 *         DO NOT MODIFY
 */
public class Consumer implements Runnable {
	Buffer buffer;

	Consumer(Buffer buffer) {
		this.buffer = buffer;
	}

	@Override
	public void run() {
		for (int i = 0; i < Main.N; i++) {
			int value = 0;
			try{
				value = buffer.get();
			}catch(InterruptedException e){}
			if (value != i) {
				System.out.println("Wrong value. I was supposed to get " + i + " but I received " + value);
				System.exit(1);
			}
		}
		System.out.println("I finished Correctly");
	}

}
/* DO NOT MODIFY */