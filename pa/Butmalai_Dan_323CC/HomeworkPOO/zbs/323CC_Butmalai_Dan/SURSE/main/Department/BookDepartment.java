package main.Department;
import main.Store.*;

public class BookDepartment  extends Department{
    /*
       Implementeaza acceptul pentru book department
     */
    public BookDepartment(String dep) {
        super(dep);
    }

    @Override
    public void accept(Visitor visit) {
        ShoppingCart cart = (ShoppingCart)visit;
        for (Item itm : getItems()) {
            int index = cart.indexOf(itm);
            if(index != -1) {
                Item curr = cart.getItem(index);
                curr.setPrice(curr.getPrice()*0.9);
            }
        }
    }
}
