package getPathSequential;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;

/**
 * @author cristian.chilipirea
 * 
 */

public class MyRunnable implements Runnable {
	ArrayList<Integer> partialPath;
	ExecutorService tpe;
	int destination, cnt = 0;

	public MyRunnable(ExecutorService tpe, ArrayList<Integer> partialPath, int dest, int cnt) {
		this.partialPath = partialPath;
		this.tpe = tpe;
		destination = dest;
		this.cnt = cnt;
	}

	@Override
	public void run() {
		if(cnt > 1000)
			tpe.shutdownNow();
		if (partialPath.get(partialPath.size() - 1) == destination) {
			System.out.println(partialPath);
			tpe.shutdownNow();
		}

		// Try to add all possible next nodes that do not create a cycle.
		int lastNodeInPath = partialPath.get(partialPath.size() - 1);
		for (int i = 0; i < Main.graph.length; i++) {
			if (Main.graph[i][0] == lastNodeInPath) {
				if (partialPath.contains(Main.graph[i][1]))
					continue;
				ArrayList<Integer> newPartialPath = (ArrayList<Integer>) partialPath.clone();
				newPartialPath.add(Main.graph[i][1]);
				tpe.submit(new MyRunnable(tpe, newPartialPath, destination, cnt ));
			}
		}
	}

}
