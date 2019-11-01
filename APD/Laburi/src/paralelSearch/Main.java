import java.util.concurrent.CyclicBarrier;
public class Main {
	public static boolean found = false;
	public static final int N = 4;
	public static int find = 35;
	static final int[] a = new int[100]; 
	static  final int[] sign = new int[4];
	static int n = 100, st=0, dr = 100;
	static int result = 0;
	public static CyclicBarrier cyclicBarrier;
	public static void main(String[] args) {
		Thread threads[] = new Thread[N];
		for(int i= 0 ; i <  n; i++){
			a[i] = i;
		}
		cyclicBarrier = new CyclicBarrier(N);
		for (int i = 0; i < N; i++)
			threads[i] = new Thread(new Philosopher(i));
		for (int i = 0; i < N; i++)
			threads[i].start();

		for (int i = 0; i < N; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		System.out.println(result);
	}

}
/* DO NOT MODIFY */