
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
/**
 * @author cristian.chilipirea
 * 
 */
public class Philosopher implements Runnable {
	int id;

	public Philosopher(int id) {
		this.id = id;
	}

	
	@Override
	public void run(){
		
		while(!Main.found){
			
			try{
				try{
					Main.cyclicBarrier.await();
				}catch(  BrokenBarrierException  e){}
			}catch(  InterruptedException  e){}

			int check_poz = (int)(((double)(Main.dr + Main.st)/((double)Main.N))*((double)id));
			Main.sign[id] = 1;
			if(Main.a[check_poz] < Main.find){
				Main.sign[id] = 0;
			}
			if(Main.a[check_poz] == Main.find){
				Main.result = check_poz;
				Main.found = true;
			}

			try{
				try{
					Main.cyclicBarrier.await();
				}catch(  BrokenBarrierException  e){}
			}catch(  InterruptedException  e){}
			if(id == 0){
				System.out.print(Main.st + " " + Main.dr);
				System.out.println();
				for(int i  = 0; i < 3; i++){
					// /System.out.print((int)(((double)(Main.dr + Main.st)/((double)Main.N))*((double)id)))
					if(Main.sign[i] == 0 && Main.sign[i+1] == 1){
						int st = Main.st;
						int dr = Main.dr;
						Main.st = (int)(((double)(dr + st)/((double)Main.N))*((double)i));
						Main.dr = (int)(((double)(dr + st)/((double)Main.N))*((double)i+1));
						break;
					}
				}
			}
			try{
				try{
					Main.cyclicBarrier.await();
				}catch(  BrokenBarrierException  e){}
			}catch(  InterruptedException  e){}
		}

	}
}
