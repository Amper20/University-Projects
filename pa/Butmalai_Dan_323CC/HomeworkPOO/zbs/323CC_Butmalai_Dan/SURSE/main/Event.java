package main;

import main.Store.*;
import main.Department.*;

import javax.swing.text.html.HTMLEditorKit;
import java.util.Vector;

public class Event {
    //rezolva fisierul event.txt specific fiecarui store
    private Vector<String> result = new Vector<>();

    public void solve(String event){

        String[] data = event.split(";");

        // done
        if(data[0].equals("addItem"))
            addItem(Integer.valueOf(data[1]), data[2], data[3]);

        //done
        if(data[0].equals("delItem"))
            delItem(Integer.valueOf(data[1]), data[2], data[3]);

        //done
        if(data[0].equals("addProduct"))
            addProduct(Integer.valueOf(data[1]), Integer.valueOf(data[2]), Double.valueOf(data[3]), data[4]);

        if(data[0].equals("modifyProduct"))
            modifyItem(Integer.valueOf(data[1]), Integer.valueOf(data[2]), Double.valueOf(data[3]));

        if(data[0].equals("delProduct"))
            delProduct(Integer.valueOf(data[1]));

        if(data[0].equals("getItem")){
            getItem(data[1]);
        }
        if(data[0].equals("getItems")){
            result.add(getItems(data[1].equals("ShoppingCart"), data[2]).toString());
        }
        if(data[0].equals("getTotal")){
            double ans = getTotal(data[1].equals("ShoppingCart"), data[2]);
            result.add(String.valueOf(Math.round(ans*100)/100.0));
        }
        if(data[0].equals("accept")){
            accept(Integer.valueOf(data[1]), data[2]);
        }
        if(data[0].equals(("getObservers"))){
            getObservers(Integer.valueOf(data[1]));
        }
        if(data[0].equals(("getNotifications"))){
            getNotification(data[1]);
        }
    }


    private void getNotification(String cust_name){
        result.add(Store.getInstance().getCustomer(cust_name).getNotifications().toString());
    }

    private void getObservers(int depID){
        for(Department dep : Store.getInstance().getDepartments()){
            if(dep.getID() == depID){
                result.add(dep.getPotential_customers().toString());
                break;
            }
        }
    }

    private double getTotal(boolean shoppingCart, String name) {
        if(shoppingCart)
            return Store.getInstance().getCustomer(name).getCart().getTotalPrice();
        else
            return Store.getInstance().getCustomer(name).getList().getTotalPrice();
    }

    private void accept(int depID, String customer_name){
        for(Department dep : Store.getInstance().getDepartments()){
            if(dep.getID() == depID){
                for(Customer cust : dep.getCustomers()){
                    if(cust.getName().equals(customer_name)){
                        dep.accept(cust.getCart());
                    }
                }
            }
        }
    }

    private Vector<Item> getItems(boolean where,String name){
        if(where)
            return  Store.getInstance().getCustomer(name).getCartAsVector();
        else
            return Store.getInstance().getCustomer(name).getListAsVector();
    }

    public void getItem(String name){
        Item getItm = Store.getInstance().getCustomer(name).getList().executeStrategy();
        for(Department dep : Store.getInstance().getDepartments()){
            if(dep.getPotential_customers() != null)
                for(Customer cust : dep.getPotential_customers()){
                    if(cust.getName().equals(name)){
                        boolean contains = false;
                        for(Item cust_itm:cust.getListAsVector())
                        {
                            for(Item dep_itm:dep.getItems()){
                                contains |= (cust_itm.getId() == dep_itm.getId());
                            }
                        }
                        if(!contains ){
                            dep.getPotential_customers().remove(cust);
                            break;
                        }
                    }
                }
        }
        result.add(getItm.toString());
        delItem(getItm.getId(),"WishList", name);
        addItem(getItm.getId(),"ShoppingCart", name);

    }



    private void addItem(int ID, String where, String name){

        Customer cust = Store.getInstance().getCustomer(name);

        Item to_add = Store.getInstance().itemTreeMap.get(ID);

        boolean result = false;
        if(where.equals("ShoppingCart"))
            result = cust.getCart().add(to_add);
        else
            result = cust.getList().add(to_add);

        for(Department dep : Store.getInstance().getDepartments()){
            if(dep.containsItem(to_add.getId())) {
                if(!dep.getObservers().contains(cust) && result)
                    dep.addObserver(cust);
                if(where.equals("ShoppingCart")){
                    if(!dep.getCustomers().contains(cust))
                        dep.addCustomer(cust);
                }else{
                    if(!dep.getPotential_customers().contains(cust))
                        dep.addPotentialCustomer(cust);
                }
                break;
            }
        }
    }

    private void delItem(int ID, String where, String cust_name){

        Customer cust = Store.getInstance().getCustomer(cust_name);
        Item item = Store.getInstance().getItem(ID);

        if(where.equals("ShoppingCart")){
            cust.getCart().remove(item);
        }else
            cust.getList().remove(item);

        for(Department dep : Store.getInstance().getDepartments()){
            if(dep.getObservers().contains(cust)) {

                boolean wish = false, cart = false;

                for(Item itm : dep.getItems()){
                    if(cust.getList().contains(itm))
                        wish = true;
                    if(cust.getCart().contains(itm))
                        cart = true;
                }

                if(!wish && !cart){
                    dep.getObservers().remove(cust);
                    dep.getCustomers().remove(cust);
                    dep.getPotential_customers().remove(cust);
                }
                if(!wish)
                    dep.getPotential_customers().remove(cust);
                if(!cart)
                    dep.getCustomers().remove(cust);
            }
        }
    }

    private void delProduct(int ItemID){

        for(int i = 0 ; i < Store.getInstance().getItems().size(); i++)
            if(Store.getInstance().getItems().get(i).getId() == ItemID)
                Store.getInstance().getItems().remove(i);

        int depID = 0;
        for (Department dep : Store.getInstance().getDepartments())
            for(Item itm : dep.getItems())
                if(itm.getId() == ItemID){
                    depID = dep.getID();
                    break;
                }
        Notification not = new Notification(Notification.NotificationType.REMOVE,Store.getInstance().getItem(ItemID),depID,1,ItemID);
        notifyObservers(not);

        for(Department dep : Store.getInstance().getDepartments()){
            for(Item itm : dep.getItems()){
                if(itm.getId() == ItemID) {
                    dep.getItems().remove(itm);
                    break;
                }
            }
        }

        for(Customer cust: Store.getInstance().getCustomers()){
            delItem(ItemID,"ShoppingCart",cust.getName());
            delItem(ItemID,"WishList",cust.getName());
        }
    }

    private void addProduct(int DepID, int ItemID, double price, String name){
        Item item = new Item(name,ItemID,price);
        Notification not = new Notification(Notification.NotificationType.ADD,item,DepID,1,ItemID);
        notifyObservers(not);
        Store.getInstance().getDepartment(DepID).addItem(item);
        Store.getInstance().addItem(item);
        Store.getInstance().itemTreeMap.put(ItemID, item);
    }

    private void modifyItem(int DepID, int ItemID, double price){
        String name = "";
        for(Item itm : Store.getInstance().getDepartment(DepID).getItems()){
            if(itm.getId() == ItemID){
                itm.setPrice(price);
                name = itm.getName();
            }
        }
        Item nw = new Item(name,ItemID, price);
        for(Department dep : Store.getInstance().getDepartments()){
            if(dep.getID() == DepID) {
                Notification not = new Notification(Notification.NotificationType.MODIFY, nw, DepID, 1, ItemID);
                dep.notifyAllObservers(not);
            }
        }
    }

    private void notifyObservers(Notification not){
        for(Department dep : Store.getInstance().getDepartments()){
            if(dep.getID() == not.getDepartmentID()){
                for(Customer cust : dep.getPotential_customers()){
                    cust.update(not);
                }
            }
        }
    }

    public Vector<String> getResult() {
        return result;
    }
}
