package main.Department;
import main.Store.*;

import static java.lang.Math.min;

public class SoftwareDepartment extends Department{
    /*
       Implementeaza acceptul pentru SoftwareDepartment
     */
    public SoftwareDepartment(String dep) {
        super(dep);
    }

    @Override
    public void accept(Visitor visit) {
        if (visit instanceof ShoppingCart){
            ShoppingCart cart = (ShoppingCart) visit;
            double mini = getItems().get(0).getPrice();
            for (Item itm : getItems()) {
                mini = min(mini, itm.getPrice());
            }
            if (mini > cart.getBudget() - cart.getTotalPrice()) {
                for (Item itm : getItems()) {
                    int index = cart.indexOf(itm);
                    if (index != -1) {
                        cart.getItem(index).setPrice(cart.getItem(index).getPrice() * 0.8);
                    }
                }
            }
        }
    }
}
