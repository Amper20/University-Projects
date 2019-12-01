package godsavedaqueen;

import getPathSequential.Main;

import java.util.concurrent.ExecutorService;

import static godsavedaqueen.NQueenProblem.putboard;
import static godsavedaqueen.NQueenProblem.unsafe;

/**
 * @author cristian.chilipirea
 * 
 */

public class MyRunnable implements Runnable {
	ExecutorService tpe;

	private int[] b = new int[8];
	int y;

	public MyRunnable(ExecutorService tpe, int[] b, int y) {
		this.tpe = tpe;
		this.b = b.clone();
		this.y = y;
	}
	@Override
	public void run() {
		do {
			b[y]++;
		} while ((b[y] < 8) && unsafe(b, y));
		if (b[y] < 8) {
			if (y < 7) {
				y++;
				b[y] = -1;
			} else {
				putboard(b);
			}
		} else {
			y--;
		}
		int[] bNew = new int[8];
		for(int i = 0 ; i < 8; i++)
			bNew[i] = b[i];
		tpe.submit(new MyRunnable(tpe, b, y));

	}

}
