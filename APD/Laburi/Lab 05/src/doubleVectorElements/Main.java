
/**
 * @author cristian.chilipirea
 *
 */
public class Main {
	static int N = 1000000;
	static int v[] = new int[N];
	static int P = 4;
	public static void main(String[] args) {
		
		for(int i=0;i<N;i++)
			v[i]=i;
		
		// for (int i = 0; i < N; i++) {
		// 	v[i] = v[i] * 2;
		// }
		NewThread threads[] = new NewThread[P];
		
		for (int i = 0; i < P; i++) {
			threads[i] = new NewThread(i);
		}
		
		for (int i = 0; i < P; i++) {
			threads[i].start();
		}
		
		for (int i = 0; i < P; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		for (int i = 0; i < N; i++) {
			if(v[i]!= i*2) {
				System.out.println("Wrong answer");
				System.exit(1);
			}
		}
		System.out.println("Correct");
	}

}
