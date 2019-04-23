package main.Department;
import main.Store.*;
import main.Strategy.*;

import java.util.Vector;

public abstract class Department implements  Subject {
    /*
        Clasa de baza departament implementeaza funtionalitatile descrise in cerinta temei, este extinsa de
         Bookdepartment, etc.
     */
    private Vector<Customer> observers;
    private Vector<Customer> customers = null;
    private Vector<Customer> potential_customers = null;
    private Vector<Item> items  = null;
    private String name;
    private int ID;

    public void enter(Customer customer){
        customers.add(customer);
    }

    public Department(String dep){
        observers = new Vector<>();
        items = new Vector<>();
        customers = new Vector<>();
        potential_customers = new Vector<>();
        ID = Integer.valueOf(dep.split(";")[1]);
        name = dep.split(";")[0];
    }

    public void exit(Customer customer){
        for(Customer it : customers)
            if(it.getName().equals(customer.getName())) {
                customers.remove(it);
                return;
            }
    }

    public boolean departmentHasItem(Item item){
        return items.contains(item);
    }

    public void addItem(Item item){
        items.add(item);
    }

    public void addCustomer(Customer customer){
        customers.add(customer);
    }

    public void addPotentialCustomer(Customer customer){
        potential_customers.add(customer);
    }

    @Override
    public void addObserver(Customer customer) {
        observers.add(customer);
    }

    @Override
    public void removeObserver(Customer customer) {
        observers.remove(customer);
    }

    @Override
    public void notifyAllObservers(Notification not) {
        for (Customer obs : potential_customers){
            obs.update(not);
        }
    }

    public Vector<Item> getItems() {
        return items;
    }

    public Vector<Customer> getCustomers() {
        return customers;
    }

    public Vector<Customer> getObservers() {
        return observers;
    }

    public Vector<Customer> getPotential_customers(){
        return potential_customers;
    }

    public abstract void accept(Visitor visitor);

    public int getID() {
        return ID;
    }

    public String getName() {
        return name;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public boolean containsItem(int ID){
        for(Item itm : items)
            if(ID == itm.getId())
                return true;
            return false;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        String ret = name  + "\nCustomers\n";
        for(Customer cust: customers){
            ret += cust.toString();
        }
        ret += "\nWishCustomers\n";
        for(Customer cust: potential_customers){
            ret += cust.toString();
        }
        ret += "\nItems\n";
        for(Item cust: items){
            ret += cust.toString();
        }
        ret += "\n\n";
        return ret;
    }
}
