
public class NewThread extends Thread {
	static int value = 0;
    public int id;
    NewThread(int id){
        this.id = id;
    }
	@Override
	public void run() {
		System.out.println(id);
	}
}
