package main.Department;
import main.Store.*;
import main.Strategy.*;

public interface Subject {
    /*
        interfata implementata in classa department
     */
    public void addObserver(Customer customer);
    public void removeObserver(Customer customer);
    public void notifyAllObservers(Notification not);
}
