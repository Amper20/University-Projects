package parallelTree;

import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Semaphore;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/**
 * @author cristian.chilipirea
 *
 */
public class ReadTreePart implements Runnable {
	TreeNode tree;
	String fileName;

	public ReadTreePart(TreeNode tree, String fileName) {
		this.tree = tree;
		this.fileName = fileName;
	}

	@Override
	public void run() {
		BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(fileName));
			String line = reader.readLine();
			while (line != null) {
				String[] arr = line.split("\t");//take the input in string array separated by whitespaces" "
				
				int child = Integer.valueOf(arr[0]);
				int parent = Integer.valueOf(arr[1]);
				while(tree.getNode(parent) == null);
				tree.getNode(parent).addChild(new TreeNode(child));

				line = reader.readLine();
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		try{
		try{Main.barrier.await();}catch(InterruptedException e){}
		}catch(BrokenBarrierException e){}
	}

}
