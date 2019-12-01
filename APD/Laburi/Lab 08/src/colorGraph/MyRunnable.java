package colorGraph;

import java.util.concurrent.ExecutorService;

import static colorGraph.Main.*;

/**
 * @author cristian.chilipirea
 * 
 */

public class MyRunnable implements Runnable {
	int[] colors;
	int step;
	ExecutorService tpe;

	public MyRunnable(ExecutorService tpe, int[] colors, int step) {
		this.colors = colors;
		this.step = step;
		this.tpe = tpe;
	}

	static void colorGraph(int[] colors, int step) {

	}

	@Override
	public void run() {
		if (step == N) {
			printColors(colors);
			tpe.shutdownNow();
		}

		// for the node at position step try all possible colors
		for (int i = 0; i < COLORS; i++) {
			int[] newColors = colors.clone();
			newColors[step] = i;
			if (verifyColors(newColors, step))
				tpe.submit(new MyRunnable(tpe, newColors, step + 1));
		}
	}

}
