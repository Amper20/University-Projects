import java.util.concurrent.*;
public class NewThread extends Thread {
	static int value = 0;
    public int id;
    NewThread(int id){
        this.id = id;
    }
	@Override
	public void run() {
		int start = ((int)((double)id*((5.0/Main.P)))); 
		int end = ((int)((double)(id+1)*((5.0/Main.P))));
		System.out.println(start);
		System.out.println(end);
		
		for (int k = 0; k < 5; k++) {	
			for (int i = start; i < end; i++) {
					for (int j = 0; j < 5; j++) {
							Main.graph[i][j] = Math.min(Main.graph[i][k] + Main.graph[k][j], Main.graph[i][j]);
					}
				}
		try {
           Main.barrier.await();
         } catch (InterruptedException ex) {
           return;
         } catch (BrokenBarrierException ex) {
           return;
         }
		}
	}
}
