package main.Store;
import main.Department.*;
import main.Strategy.*;
public class WishList extends ItemList implements Visitor {
    //adauga functionalitatea necesara ItemListului pentru a obtine un wish list
    private Strategy strategy;

    public void setStrategy(Strategy strategy) {
        this.strategy = strategy;
    }

    public Strategy getStrategy() {
        return strategy;
    }

    public Item executeStrategy(){
        Item ret =  strategy.execute(this);
        return ret;
    }

    @Override
    public void visit(BookDepartment bookDepartment) {
        bookDepartment.accept(this);
    }

    @Override
    public void visit(MusicDepartment musicDepartment) {
        musicDepartment.accept(this);
    }

    @Override
    public void visit(SoftwareDepartment softwareDepartment) {
        softwareDepartment.accept(this);
    }

    @Override
    public void visit(VideoDepartment videoDepartment) {
        videoDepartment.accept(this);
    }

    @Override
    public String toString() {
        return super.toString();
    }
}
