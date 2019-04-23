package main.Fragments;

import main.Event;
import main.Store.Customer;
import main.Store.Item;
import main.Store.ItemList;
import main.Store.Store;
import main.Test;
import main.UImain;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.rmi.activation.ActivationID;
import java.util.Vector;

public class CustomerOverview extends Fragment{
    /*
        CustomerOverview descrie fragmentul(pagina) pentru vizualizarea listei clientilor.
    */
    DefaultListModel<String> list;
    JList<String> dispList;
    JScrollPane listPane;
    JButton selectCustomerWishList, selectCustomerShoppingCart;
    GroupLayout layout;
    ActionListener onSelectCustomerWishList, onSelectCustomerShoppingCart;
    UImain callback;
    JLabel customerLabel;

    public CustomerOverview(int ID, UImain callback) {

        super(ID, callback);
        this.callback = callback;

        backButton = new JButton("Back");
        backButton.addActionListener(callback);
        selectCustomerWishList = new JButton("Get Customers Wish List");
        selectCustomerShoppingCart = new JButton("Get Customers Shopping Cart");

        customerLabel = new JLabel("Costumers Overview");
        customerLabel.setFont(new Font(customerLabel.getName(), Font.PLAIN, 28));

        list = new DefaultListModel<>();
        dispList = new JList<>(list);
        listPane = new JScrollPane(dispList);
        updateList();

        setLayout();

        setListeners();
    }

    private void setListeners() {
        onSelectCustomerShoppingCart = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectCustomerWishList.setBackground(new JButton().getBackground());
                if(dispList.getSelectedValue() == null){
                    selectCustomerShoppingCart.setBackground(new Color(255,0,0));
                    return;
                }else
                    selectCustomerShoppingCart.setBackground(new Color(0,255,0));

                Customer cust = Store.getInstance().getCustomer(dispList.getSelectedValue());
                ((CartView)callback.getFragment(6)).setCustomer(cust);
                ((CartView)callback.getFragment(6)).updateList();
                callback.changeFragment(callback.getFragment(6));
            }
        };

        onSelectCustomerWishList = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectCustomerShoppingCart.setBackground(new JButton().getBackground());
                dispList.getSelectedValue();
                if(dispList.getSelectedValue() == null){
                    selectCustomerWishList.setBackground(new Color(255,0,0));
                    return;
                }else
                    selectCustomerWishList.setBackground(new Color(0,255,0));
                Customer cust = Store.getInstance().getCustomer(dispList.getSelectedValue());
                ((WishView)callback.getFragment(7)).setCustomer(cust);
                callback.changeFragment(callback.getFragment(7));
            }
        };

        selectCustomerShoppingCart.addActionListener(onSelectCustomerShoppingCart);
        selectCustomerWishList.addActionListener(onSelectCustomerWishList);
    }

    public void setLayout(){
        layout = new GroupLayout(getPanel());
        layout.setAutoCreateContainerGaps(true);
        layout.setAutoCreateGaps(true);
        layout.setHorizontalGroup(
                layout.createParallelGroup()
                    .addComponent(customerLabel)
                    .addGroup(
                            layout.createSequentialGroup()
                                .addComponent(backButton)
                                .addComponent(selectCustomerShoppingCart)
                                .addComponent(selectCustomerWishList)
                    )
                    .addComponent(listPane)
        );
        layout.setVerticalGroup(
                layout.createSequentialGroup()
                        .addComponent(customerLabel)
                        .addGroup(
                                layout.createParallelGroup()
                                        .addComponent(backButton)
                                        .addComponent(selectCustomerShoppingCart)
                                        .addComponent(selectCustomerWishList)
                        )
                        .addComponent(listPane)
        );
        getPanel().setLayout(layout);
    }

    public void updateList(){
        list.clear();
        for(Customer cust : Store.getInstance().getCustomers())
            list.addElement(cust.toString());
    }
}
