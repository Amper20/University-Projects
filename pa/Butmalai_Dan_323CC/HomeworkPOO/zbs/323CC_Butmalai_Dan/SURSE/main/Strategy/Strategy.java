package main.Strategy;
import main.Store.*;

public interface Strategy {
    //Interfata ce e folosita pentru a implementa Strategy pattern
    public abstract Item execute(WishList list);
}
