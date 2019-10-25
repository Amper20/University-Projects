
public class Main {
	public static int N = 4;
	public static void main(String[] args) {
		NewThread threads[] = new NewThread[N];
		
		for (int i = 0; i < N; i++) {
			threads[i] = new NewThread(i);
		}
		
		for (int i = 0; i < N; i++) {
			threads[i].start();
		}
		
		System.out.println("Hello from main thread");
		
		for (int i = 0; i < N; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

	}

}
