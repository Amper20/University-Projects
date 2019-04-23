package main.Fragments;

import main.UImain;

import javax.swing.*;

public class Fragment{

    /*
        Fragment descrie fragmentul general si contine un builder care returneaza fiecare subfragment separat.
    */

    private JPanel panel;
    int ID;
    JButton backButton = new JButton("Back");

    public Fragment(int ID, UImain callback){
        panel = new JPanel();
        this.ID = ID;
    }
    public static Fragment getNewFragment(int ID, UImain callback){
        //fiecare fragment are un id
        if(ID == 0)
            return new Loader(ID, callback);
        if(ID == 1)
            return new StoreOverview(ID, callback);
        if(ID == 2)
            return new ItemsOverview(ID, callback);
        if(ID == 3)
            return new ItemsList(ID, callback);
        if(ID == 4)
            return new ItemEdit(ID, callback);
        if(ID == 5)
            return new CustomerOverview(ID, callback);
        if(ID == 6)
            return new CartView(ID, callback);
        if(ID == 7)
            return new WishView(ID, callback);
        return null;
    }

    public JPanel getPanel() {
        return panel;
    }
}
