package main.Department;
import main.Store.*;

public class MusicDepartment extends  Department{
    /*
       Implementeaza acceptul pentru musicDepartent
     */
    public MusicDepartment(String dep) {
        super(dep);
    }

    @Override
    public void accept(Visitor visit) {
        ShoppingCart cart = (ShoppingCart)visit;
        double profit = 0.0;
        for (Item itm : getItems()) {
            int index = cart.indexOf(itm);
            if(index != -1) {
                profit += cart.getItem(index).getPrice();
            }
        }
        cart.setBudget(cart.getBudget() + 0.1 * profit);
    }
}
