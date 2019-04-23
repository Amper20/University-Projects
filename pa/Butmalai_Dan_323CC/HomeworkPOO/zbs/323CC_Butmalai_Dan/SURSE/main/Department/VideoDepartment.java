package main.Department;
import main.Store.*;

import static java.lang.Math.max;

public class VideoDepartment extends Department{
    /*
           Implementeaza acceptul pentru Videodepartment
    */
    public VideoDepartment(String dep) {
        super(dep);
    }

    @Override
    public void accept(Visitor visit) {
        ShoppingCart cart = (ShoppingCart)visit;
        double sum = 0.0, maxi = -1.0;
        for(Item it : getItems()) {
            if (cart.contains(it)) {
                sum += it.getPrice();
                cart.setBudget(cart.getBudget() + it.getPrice() * 0.05);
            }
            maxi = max(maxi, it.getPrice());
        }
        if(sum > maxi){
            for(Item it : getItems()) {
                if (cart.contains(it))
                    cart.getItem(cart.indexOf(it)).setPrice(cart.getItem(cart.indexOf(it)).getPrice() * 0.85);
            }
        }
    }
}
