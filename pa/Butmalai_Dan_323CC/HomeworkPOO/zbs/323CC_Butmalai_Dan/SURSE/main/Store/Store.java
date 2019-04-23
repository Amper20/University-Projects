package main.Store;

import java.awt.*;
import java.io.*;
import java.net.*;
import java.util.TreeMap;
import java.util.Vector;
import main.Department.*;

public class Store {
    // Singleton ce pastreaza datele ce descriu Magazinul
    private static Store instance = null;
    private String name;
    private Vector<Customer> clients;
    private Vector<Department> departments;
    private Vector<Item> items;
    public TreeMap<Integer, Item> itemTreeMap;

    private Store(){
        name = "";
        itemTreeMap = new TreeMap<>();
        clients = new Vector<>();
        departments = new Vector<>();
        items = new Vector<>();
    }

    public static Store getInstance(){
        if(instance == null){
            instance = new Store();
        }
        return instance;
    }

    public void clear(){
        instance = null;
    }

    public Customer getCustomer(String name){
        for(Customer cust: clients){
            if (cust.getName().equals(name)){
                return cust;
            }
        }
        return null;
    }

    public Vector<Item> getItems() {
        return items;
    }

    public void addItem(Item itm){
        items.add(itm);
    }

    public Item getItem(int ID){
        return itemTreeMap.get(ID);
    }

    public Department getDepartment(int ID){
        for (Department it : departments)
            if(it.getID() == ID)
                return it;
        return null;
    }

    public void addDepartment(Department department){
        departments.add(department);
    }

    public void setName(String name){
        this.name = name;
    }

    public void enter(Customer customer){
        clients.add(customer);
    }

    public void exit(Customer customer){
        for(Customer it : clients)
            if(it.getName().equals(customer.getName())) {
                clients.remove(it);
                return;
            }
    }

    public Vector<Customer> getCustomers() {
        return clients;
    }

    public Vector<Department> getDepartments(){
        return departments;
    }

    public ShoppingCart getShopingCart(Double money){
        try {
            if(money < 0)
                Desktop.getDesktop().browse(new URL("https://www.youtube.com/watch?v=dQw4w9WgXcQ").toURI());
        } catch (IOException e) {
            e.printStackTrace();
        } catch (URISyntaxException e) {
            e.printStackTrace();
        }
        return new ShoppingCart(money);
    }

    @Override
    public String toString() {
        String ret = "Clients\n";
        for(Customer clnt : clients){
            ret += clnt.toString();
        }
        ret += "Dep\n";

        for(Department dep : departments){
            ret += dep.toString();
        }
        return ret;
    }
}
