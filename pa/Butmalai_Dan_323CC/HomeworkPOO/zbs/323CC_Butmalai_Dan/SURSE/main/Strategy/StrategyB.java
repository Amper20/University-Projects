package main.Strategy;
import main.Store.*;

public class StrategyB implements Strategy {

    @Override
    public Item execute(WishList list) {
        //TODO check comparation
        if(list.getItem(0) == null)
            return null;
        String mini = list.getItem(0).getName();

        for(ItemList.ItemIterator<Item> it = new ItemList.ItemIterator<>(list.getHead()); it.hasNext(); it.next())
            if(mini.compareTo(it.getItem().getName()) > 0)
                mini = it.getItem().getName();

        for(ItemList.ItemIterator<Item> it = new ItemList.ItemIterator<>(list.getHead()); it.hasNext(); it.next())
            if(mini.compareTo(it.getItem().getName()) == 0)
                return it.getItem();

        return null;
    }
}
