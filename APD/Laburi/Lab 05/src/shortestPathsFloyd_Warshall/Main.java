import java.util.concurrent.*;
public class Main  {
	static double P = 4;
	static int M = 9;
	static int graph[][] = { { 0, 1, M, M, M }, 
				          { 1, 0, 1, M, M }, 
				          { M, 1, 0, 1, 1 }, 
				          { M, M, 1, 0, M },
				          { M, M, 1, M, 0 } };
	public static CyclicBarrier barrier = new CyclicBarrier((int)P);
	public static void main(String[] args) {
		
		// Parallelize me (You might want to keep the original code in order to compare)
		NewThread threads[] = new NewThread[(int)P];
		
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

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph[i][j] + " ");
			}
			System.out.println();
		}
	}
}
