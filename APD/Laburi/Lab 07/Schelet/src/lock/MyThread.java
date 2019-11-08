package lock;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Semaphore;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import java.util.concurrent.CyclicBarrier;
/**
 * @author cristian.chilipirea
 * 
 */
public class MyThread implements Runnable {
	int id;
	static int value = 0;
	MyLock lock;

	MyThread(int id, MyLock lock) {
		this.id = id;
		this.lock = lock;
	}

	@Override
	public void run() {
		if (id == 0) {
			try {Main.available.acquire(1);}catch (InterruptedException e) {};
			BufferedReader reader;
			try {
				reader = new BufferedReader(new FileReader("a"));
				String line = reader.readLine();
				while (line != null) {
					System.out.println("0-"+line);
					try {Main.a.put(line);}catch (InterruptedException e) {};
					line = reader.readLine();
				}
				reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			Main.available.release(1);
		} 
		if(id == 1){
			try {Main.available.acquire(1);}catch (InterruptedException e) {};
			BufferedReader reader;
			try {
				reader = new BufferedReader(new FileReader("b"));
				String line = reader.readLine();
				while (line != null) {
					System.out.println("1-"+line);
					try {Main.a.put(line);}catch (InterruptedException e) {};
					line = reader.readLine();
				}
				reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			Main.available.release(1);
		}
		if(id == 2){
			try {Main.available.acquire(1);}catch (InterruptedException e) {};
			BufferedReader reader;
			try {
				reader = new BufferedReader(new FileReader("c"));
				String line = reader.readLine();
				while (line != null) {
					System.out.println("2-"+line);
					try {Main.a.put(line);}catch (InterruptedException e) {};

					line = reader.readLine();
				}
				reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			Main.available.release(1);
		}
		if(id == 3){
			try {Main.available.acquire(3);}catch (InterruptedException e) {};
			System.out.println("-------alabala-----");
			System.out.println(Main.a.toString());
			
			Main.available.release(3);
		}
	}
}
