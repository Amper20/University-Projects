import java.util.ArrayList;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Class that implements the channel used by headquarters and space explorers to communicate.
 */
public class CommunicationChannel {
    private ArrayBlockingQueue<Message> messSpaceExpl;
    private ArrayBlockingQueue<Message> messHeadQuarter;
    private ConcurrentHashMap<Long, ArrayList<Message>> map;
    private AtomicInteger sendId;
    /**
     * Creates a {@code CommunicationChannel} object.
     */
    public CommunicationChannel() {
        messHeadQuarter = new ArrayBlockingQueue<Message>(100000);
        messSpaceExpl = new ArrayBlockingQueue<Message>(100000);
        map = new ConcurrentHashMap<>();
        sendId = new AtomicInteger(-1);
    }


    /**
     * Puts a message on the space explorer channel (i.e., where space explorers write to and
     * headquarters read from).
     *
     * @param message
     *            message to be put on the channel
     */
    public void putMessageSpaceExplorerChannel(Message message) {
        try {
            messSpaceExpl.put(message);
        } catch (InterruptedException e) {
        }
    }

    /**
     * Gets a message from the space explorer channel (i.e., where space explorers write to and
     * headquarters read from).
     *
     * @return message from the space explorer channel
     */
    public Message getMessageSpaceExplorerChannel() {
        try {
            return messSpaceExpl.take();
        } catch (InterruptedException e) {
            return null;
        }
    }

    private void sendToExplorers(long tid){

        ArrayList<Message> list = map.get(tid);
        if(list != null){
            for (int i = 0 ; i < list.size(); i += 2) {
                try {
                    Message mes1 = list.get(i);
                    Message mes2 = list.get(i + 1);
                    mes2.setParentSolarSystem(mes1.getCurrentSolarSystem());
                    messHeadQuarter.put(mes2);
                } catch (InterruptedException e) {
                    return;
                }
            }
        }
        if(list.size() != 0) {
            list = new ArrayList<Message>();
            map.put(tid, list);
        }
    }


    /**
     * Puts a message on the headquarters channel (i.e., where headquarters write to and
     * space explorers read from).
     *
     * @param message
     *            message to be put on the channel
     */

    void putMessageHeadQuarterChannel(Message message) {

        long tid = Thread.currentThread().getId();
        ArrayList<Message> list = map.get(tid);
        if(list!= null && list.size()%2 == 0 && list.size() >= 10){
            sendToExplorers(Thread.currentThread().getId());
        }
        if(message.getData().equals("EXIT")){
            list.add(message);
            list.add(message);
            map.put(tid, list);
            sendToExplorers(Thread.currentThread().getId());
            return;
        }
        if(message.getData().equals("END")){
            sendToExplorers(Thread.currentThread().getId());
            return;
        }
        if(list == null)
            list = new ArrayList<>();
        list.add(message);
        map.put(tid, list);


    }

    /**
     * Gets a message from the headquarters channel (i.e., where headquarters write to and
     * space explorer read from).
     *
     * @return message from the header quarter channel
     */
    public Message getMessageHeadQuarterChannel() {
        try {
            return messHeadQuarter.take();
        } catch (InterruptedException e) {
            int dummy = 0;
        }
        return null;
    }
}
