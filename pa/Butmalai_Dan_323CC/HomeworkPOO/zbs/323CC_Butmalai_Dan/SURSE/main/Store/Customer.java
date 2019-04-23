package main.Store;
import main.Department.Department;
import main.Store.*;
import main.Strategy.*;

import javax.swing.text.html.HTMLEditorKit;
import java.util.Vector;

public class Customer implements Observer {
    //Implementeaza conceptul de customer
    private String name;
    private ShoppingCart cart;
    private WishList list;
    private Vector<Notification> notifications;
    private int time = 0;
    Strategy strategy;

    public Vector<Notification> getNotifications() {
        return notifications;
    }

    public Vector<Item> getCartAsVector(){
        Vector<Item> ret = new Vector<>();
        for(int i = 0 ; i < cart.getSize(); i++){
            ret.add(cart.getItem(i));
        }
        return ret;
    }

    public Vector<Item> getListAsVector(){
        Vector<Item> ret = new Vector<>();
        for(int i = 0 ; i < list.getSize(); i++){
            ret.add(list.getItem(i));
        }
        return ret;
    }

    public Customer(ShoppingCart cart, String name, String strat){
        this.name = name;
        list = new WishList();
        this.cart = cart;
        notifications = new Vector<>();

        Strategy strategy = null;
        if(strat.equals("A"))
            strategy = new StrategyA();

        if(strat.equals("B"))
            strategy = new StrategyB();

        if(strat.equals("C"))
            strategy = new StrategyC();

        list.setStrategy(strategy);
    }

    public Customer(String[] data){
        this(new ShoppingCart(data[1]), data[0], data[2]);
    }

    public void setName(String name){
        this.name = name;
    }

    public String getName(){
        return name;
    }

    public void update(Notification message){
        message.setTimeline(time);
        time++;
        if(message.display()){
            for(Department dep : Store.getInstance().getDepartments()){
                if(dep.getID() == message.getDepartmentID()){
                    for(Item itm : dep.getItems()){
                        if(list.contains(itm)) {
                            notifications.add(message);
                            break;
                        }
                    }
                }
            }
        }

        if(message.getType() == Notification.NotificationType.MODIFY){
            Department dep = Store.getInstance().getDepartment(message.getDepartmentID());

            for(Item itm: dep.getItems()){
                if(itm.getId() == message.getItemID())
                    itm.setPrice(message.getModified().getPrice());
            }

            for (ItemList.ItemIterator<Item> itm = new ItemList.ItemIterator<>(cart.getHead()); itm.hasNext();itm.next()){
                if(itm.getItem().getId() == message.getItemID())
                    itm.getItem().setPrice(message.getModified().getPrice());
            }

            for (ItemList.ItemIterator<Item> itm = new ItemList.ItemIterator<>(list.getHead()); itm.hasNext();itm.next()){
                if(itm.getItem().getId() == message.getItemID())
                    itm.getItem().setPrice(message.getModified().getPrice());
            }
        }
    }

    public void setCart(ShoppingCart cart){
        this.cart = cart;
    }


    public void setList(WishList list){
        this.list = list;
    }

    public ShoppingCart getCart(){
        return cart;
    }

    public WishList getList(){
        return list;
    }

    @Override
    public String toString() {
        return name;
    }
}
