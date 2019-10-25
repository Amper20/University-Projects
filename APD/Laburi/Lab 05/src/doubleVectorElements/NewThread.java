
public class NewThread extends Thread {
	static int value = 0;
    public int id;
    NewThread(int id){
        this.id = id;
    }
	@Override
	public void run() {
		int start = id*(Main.N/Main.P); 
		int end = (id+1)*(Main.N/Main.P);
		for(int i = start;  i < end; i++){
			Main.v[i]*=2;
		}
	}
}
