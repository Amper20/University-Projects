package main.Strategy;
import main.Store.*;

import static java.lang.Math.max;

public class StrategyC implements Strategy {
    @Override
    public Item execute(WishList list) {

        int latest = 0;

        for(ItemList.ItemIterator<Item> it = new ItemList.ItemIterator<>(list.getHead()); it.hasNext(); it.next())
            latest = max(latest,it.getCurrent().getTimeline());

        for(ItemList.ItemIterator<Item> it = new ItemList.ItemIterator<>(list.getHead()); it.hasNext(); it.next())
            if(latest == it.getCurrent().getTimeline())
                return it.getItem();

        return list.getItem(0);
    }
}
