package main.Store;
import main.Department.*;
import main.Strategy.*;

public class ShoppingCart extends ItemList implements Visitor {
    //adauga functionalitatea necesara ItemListului pentru a obtine un shopping cart
    private double budget = 0.0;

    public ShoppingCart(double money){
        this.budget = money;
    }

    public ShoppingCart(String money){
        this(Double.valueOf(money));
    }

    public ShoppingCart() {}

    public void setBudget(double budget) {
        this.budget = budget;
    }

    public double getBudget() {
        return budget;
    }

    public boolean add( Item element ){
        if(budget >= getTotalPrice() + element.getPrice())
            return  super.add(element);
        else
            return false;
    };

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
        return super.toString() + String.valueOf(budget);
    }
}
