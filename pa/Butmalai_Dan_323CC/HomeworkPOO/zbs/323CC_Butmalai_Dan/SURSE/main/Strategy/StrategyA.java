package main.Strategy;
import main.Store.*;
public class StrategyA implements Strategy{
    @Override
    public Item execute(WishList list) {
        return list.remove(0);
    }
}
