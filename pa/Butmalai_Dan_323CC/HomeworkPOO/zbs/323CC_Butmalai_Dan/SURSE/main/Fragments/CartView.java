package main.Fragments;

import main.Store.Customer;
import main.Store.Item;
import main.Store.ShoppingCart;
import main.Store.Store;
import main.Test;
import main.UImain;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class CartView extends Fragment{
        /*
            CartView descrie fragmentul(pagina) pentru vizualizarea shopping cartului.
         */
        Customer customer;
        UImain callback;

        JLabel total, budget;
        JTextField addItemId;
        JButton addItem, delSelectedItem, order;

        ActionListener onAddItem, onDelSelectedItem, onOredr;

        DefaultListModel<String> model;
        JList<String> list;
        JScrollPane listPanel;
        JPanel textPanel;

        GroupLayout layout;

        public CartView(int ID, UImain callback) {
            super(ID, callback);
            this.callback = callback;

            addItem = new JButton("Add Item by ID");
            delSelectedItem = new JButton("Delete Selected Item");
            order = new JButton("Order Items");
            total = new JLabel();
            budget = new JLabel();

            addItemId = new JTextField();
            addItemId.setMaximumSize(new Dimension(220,28));
            model = new DefaultListModel<>();
            list = new JList<>(model);
            listPanel = new JScrollPane(list);
            updateList();

            setListeners();

            setLayout();

        }

    private void setLayout() {
        layout = new GroupLayout(getPanel());
        layout.setAutoCreateContainerGaps(true);
        layout.setAutoCreateGaps(true);
        layout.setVerticalGroup(
                layout.createSequentialGroup()
                        .addComponent(backButton)
                        .addGroup(layout.createParallelGroup()
                                .addComponent(order)
                                .addComponent(budget)
                                .addComponent(total)
                        )

                        .addComponent(delSelectedItem)
                        .addGroup(layout.createParallelGroup()
                                .addComponent(addItem)
                                .addComponent(addItemId))
                        .addComponent(listPanel)

        );
        layout.setHorizontalGroup(
                layout.createParallelGroup()
                        .addComponent(backButton)
                        .addGroup(layout.createSequentialGroup()
                                .addComponent(order)
                                .addComponent(budget)
                                .addComponent(total)
                        )
                        .addComponent(delSelectedItem)
                        .addGroup(layout.createSequentialGroup()
                                .addComponent(addItem)
                                .addComponent(addItemId))
                        .addComponent(listPanel)

        );
        getPanel().setLayout(layout);
    }

    private void setListeners() {
            onAddItem = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    resetButtons();
                    String ID = addItemId.getText();
                    boolean valid = false;
                    for(Item itm : Store.getInstance().getItems()){
                        if (itm.getId() == Integer.valueOf(ID))
                            valid = true;
                    }
                    if(valid){
                        if(ID.compareTo("") == 0)
                            addItem.setBackground(new Color(255,0,0));
                        else
                            addItem.setBackground(new Color(0,255,0));
                        //addItem;403;ShoppingCart;Maria
                        Test.getInstance().getEventUI().solve("addItem;" + ID + ";ShoppingCart;" + customer.getName());
                        updateList();
                    }else{
                        addItem.setBackground(new Color(255,0,0));
                    }
                }

            };

            onDelSelectedItem = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    resetButtons();
                    if(list.getSelectedValue() == null){
                        delSelectedItem.setBackground(new Color(255,0,0));
                    }else {
                        delSelectedItem.setBackground(new Color(0, 255, 0));
                        Test.getInstance().getEventUI().solve("delItem;" + list.getSelectedValue().split(";")[1] + ";ShoppingCart;" + customer.getName());
                    }
                    updateList();
                }
            };

            onOredr = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    resetButtons();
                    if(customer.getCart().getTotalPrice() < 0)
                        Store.getInstance().getShopingCart(-1.0);
                    customer.setCart(new ShoppingCart(customer.getCart().getBudget() - customer.getCart().getTotalPrice()));
                    updateList();
                }
            };

            backButton.addActionListener(callback);
            order.addActionListener(onOredr);
            addItem.addActionListener(onAddItem);
            delSelectedItem.addActionListener(onDelSelectedItem);
    }

    public void updateList(){
        model.clear();
        if(customer != null && customer.getCartAsVector() != null){
            for(Item itm : customer.getCartAsVector()){
                model.addElement(itm.toString());
            }
            budget.setText("Budget: " + String.valueOf(customer.getCart().getBudget()));
            total.setText("Current Price: " + String.valueOf(customer.getCart().getTotalPrice()));
        }else
            if(customer != null) {
                budget.setText("Budget: " + customer.getCart().getBudget());
                total.setText("Current Price: 0.0");
            }
    }

    public void setCustomer(Customer customer) {
            this.customer = customer;
    }
    public void resetButtons(){
        addItem.setBackground(new JButton().getBackground());
        delSelectedItem.setBackground(new JButton().getBackground());
        order.setBackground(new JButton().getBackground());
        backButton.setBackground(new JButton().getBackground());
    }
}
