package main.Fragments;

import main.Store.Customer;
import main.Store.Item;
import main.Store.Store;
import main.Test;
import main.UImain;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class WishView extends Fragment{
    /*
        Fragment ce implementeaza pagina de vizualizare a wishListului
     */
    Customer customer;
    UImain callback;

    JTextField addItemId;
    JButton addItem, delSelectedItem;

    ActionListener onAddItem, onDelSelectedItem;

    DefaultListModel<String> model;
    JList<String> list;
    JScrollPane listPanel;

    GroupLayout layout;

    public WishView(int ID, UImain callback) {
        super(ID, callback);
        this.callback = callback;

        addItem = new JButton("Add Item by ID");
        delSelectedItem = new JButton("Delete Selected Item");

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
                        .addComponent(delSelectedItem)
                        .addGroup(layout.createParallelGroup()
                                .addComponent(addItem)
                                .addComponent(addItemId))
                        .addComponent(listPanel)

        );
        layout.setHorizontalGroup(
                layout.createParallelGroup()
                        .addComponent(backButton)
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
                    else {
                        addItem.setBackground(new Color(0, 255, 0));
                        Test.getInstance().getEventUI().solve("addItem;" + ID + ";WishList;" + customer.getName());
                    }
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
                    Test.getInstance().getEventUI().solve("delItem;" + list.getSelectedValue().split(";")[1] + ";WishList;" + customer.getName());
                }
                updateList();
            }
        };


        backButton.addActionListener(callback);
        addItem.addActionListener(onAddItem);
        delSelectedItem.addActionListener(onDelSelectedItem);
    }

    public void updateList(){
        model.clear();
        if(customer != null && customer.getListAsVector() != null){
            for(Item itm : customer.getListAsVector())
                model.addElement(itm.toString());
        }
    }

    public void setCustomer(Customer customer) {
        this.customer = customer;
    }
    public void resetButtons(){
        addItem.setBackground(new JButton().getBackground());
        delSelectedItem.setBackground(new JButton().getBackground());
        backButton.setBackground(new JButton().getBackground());
    }
}
