package godsavedaqueen;


import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/* Java program to solve N Queen Problem using
   backtracking */
public class NQueenProblem {
    private static int[] b = new int[8];
    private static int s = 0;

    static boolean unsafe(int[] b,int y) {
        int x = b[y];
        for (int i = 1; i <= y; i++) {
            int t = b[y - i];
            if (t == x ||
                    t == x - i ||
                    t == x + i) {
                return true;
            }
        }

        return false;
    }

    public static void putboard(int []b ) {
        System.out.println("\n\nSolution " + (++s));
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                System.out.print((b[y] == x) ? "|Q" : "|_");
            }
            System.out.println("|");
        }
    }

    public static void main(String[] args) {
        int y = 0;
        b[0] = -1;
        ExecutorService tpe = Executors.newFixedThreadPool(4);
        tpe.submit(new MyRunnable(tpe, b, y));
    }
}
// This code is contributed by Abhishek Shankhadhar
