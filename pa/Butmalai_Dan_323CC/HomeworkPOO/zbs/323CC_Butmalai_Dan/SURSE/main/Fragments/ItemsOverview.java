package main.Fragments;

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

public class ItemsOverview extends Fragment {
    /*
        ItemsOverview e un Fragment care implementeaza vizualizarea generala a produselor din magazin
     */
    JLabel itemsLabel;

    JButton showItemsList;

    JButton editItem;
    JTextField priceEdit, idEdit;
    JLabel priceLEdit,idLEdit;

    JButton addItem;
    JTextField name, price, id, depID;
    JLabel nameL,priceL,idL, depIDL;

    GroupLayout layout;

    ActionListener onShowItemsList, onEditItem, onAddItem;

    UImain callback;

    public ItemsOverview(int ID, UImain callback) {
        super(ID, callback);

        this.callback = callback;

        getPanel().add(backButton);
        backButton.addActionListener(callback);

        itemsLabel= new JLabel("Items Overview");
        itemsLabel.setFont(new Font(itemsLabel.getName(), Font.PLAIN, 28));

        showItemsList =  new JButton("Show Items List");

        editItem = new JButton("Edit Item");
        priceEdit = new JTextField();
        priceEdit.setMaximumSize(new Dimension(220,28));
        idEdit = new JTextField();
        idEdit.setMaximumSize(new Dimension(220,28));
        priceLEdit = new JLabel("Price:");
        idLEdit = new JLabel("ID:");

        addItem = new JButton("Add Item");
        name = new JTextField();
        price = new JTextField();
        id = new JTextField();
        depID = new JTextField();
        name.setMaximumSize(new Dimension(220,28));
        price.setMaximumSize(new Dimension(220,28));
        id.setMaximumSize(new Dimension(220,28));
        depID.setMaximumSize(new Dimension(220,28));
        nameL = new JLabel("Name:");
        priceL = new JLabel("Price:");
        idL = new JLabel("ID:");
        depIDL = new JLabel("Dep ID");

        setLayout();

        setListeners();
    }

    private void setListeners() {
        onShowItemsList = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ((ItemsList)callback.getFragment(3)).updateList();
                callback.changeFragment(callback.getFragment(3));
            }
        };

        onEditItem = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String iD = idEdit.getText();
                String pr = priceEdit.getText();
                boolean notGood = false;
                if(iD.compareTo("")!=0 && pr.compareTo("")!=0) {
                    int Id = Integer.parseInt(iD);
                    for (Item itm : Store.getInstance().getItems())
                        if (Id == itm.getId()) {
                            editItem.setBackground(new Color(0, 255, 0));
                            notGood = true;
                            itm.setPrice(Double.valueOf(pr));
                            for (Customer cust : Store.getInstance().getCustomers()) {
                                for (ItemList.ItemIterator<Item> it = new ItemList.ItemIterator<>(cust.getCart().getHead()); it.hasNext(); it.next()) {
                                    if (it.getItem().getId() == Id)
                                        it.getItem().setPrice(Double.valueOf(pr));
                                }
                                for (ItemList.ItemIterator<Item> it = new ItemList.ItemIterator<>(cust.getList().getHead()); it.hasNext(); it.next()) {
                                    if (it.getItem().getId() == Id)
                                        it.getItem().setPrice(Double.valueOf(pr));
                                }
                            }
                            break;
                        }
                }
                if(!notGood) {
                    editItem.setBackground(new Color(255,0,0));
                }
            }
        };

        onAddItem = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String dep = depID.getText();
                String iD = id.getText();
                String nm = name.getText();
                String pr = price.getText();

                boolean notGood = false;

                if(dep.compareTo("")!=0 && iD.compareTo("")!=0 && nm.compareTo("")!=0 && pr.compareTo("")!=0) {
                    int Id = Integer.valueOf(iD);
                    for (Item itm : Store.getInstance().getItems())
                        if (Id == itm.getId()) {
                            addItem.setBackground(new Color(255, 0, 0));
                            notGood = true;
                        }
                }
                if(!notGood && dep.compareTo("")!=0 && iD.compareTo("")!=0 && nm.compareTo("")!=0 && pr.compareTo("")!=0) {
                    Test.getInstance().getEventUI().solve("addProduct;" + dep + ";" + iD + ";" + pr + ";" + nm);
                    addItem.setBackground(new Color(0,255,0));
                }else{
                    addItem.setBackground(new Color(255, 0, 0));
                }
            }
        };

        addItem.addActionListener(onAddItem);
        editItem.addActionListener(onEditItem);
        showItemsList.addActionListener(onShowItemsList);

    }

    private void setLayout() {

        layout = new GroupLayout(getPanel());
        layout.setAutoCreateContainerGaps(true);
        layout.setAutoCreateGaps(true);
        getPanel().setLayout(layout);

        layout.setVerticalGroup(
                layout.createSequentialGroup()
                        .addComponent(itemsLabel)
                        .addGroup(layout.createParallelGroup()
                                .addComponent(backButton)
                                .addComponent(showItemsList)
                        )
                        .addGroup(layout.createSequentialGroup()
                                .addComponent(editItem)
                                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                        .addComponent(idLEdit)
                                        .addComponent(idEdit))
                                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                        .addComponent(priceLEdit)
                                        .addComponent(priceEdit))
                        )
                        .addGroup(layout.createSequentialGroup()
                                .addComponent(addItem)
                                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                        .addComponent(nameL)
                                        .addComponent(name))
                                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                        .addComponent(priceL)
                                        .addComponent(price))
                                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                        .addComponent(idL)
                                        .addComponent(id))
                                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                        .addComponent(depID)
                                        .addComponent(depIDL))
                        )
        );

        layout.setHorizontalGroup(
                layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup()
                                .addComponent(itemsLabel)
                                .addGroup(layout.createSequentialGroup()
                                        .addComponent(backButton)
                                        .addComponent(showItemsList)
                                )
                                .addGroup(layout.createSequentialGroup()
                                        .addGroup(layout.createParallelGroup()
                                                .addGroup(layout.createSequentialGroup()
                                                        .addComponent(editItem)
                                                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                                                .addComponent(idLEdit)
                                                                .addComponent(priceLEdit))
                                                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                                                .addComponent(priceEdit)
                                                                .addComponent(idEdit))
                                                )
                                                .addGroup(layout.createSequentialGroup()
                                                        .addComponent(addItem)
                                                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                                                .addComponent(nameL)
                                                                .addComponent(priceL)
                                                                .addComponent(idL)
                                                                .addComponent(depIDL))
                                                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.CENTER)
                                                                .addComponent(name)
                                                                .addComponent(price)
                                                                .addComponent(id)
                                                                .addComponent(depID))
                                                )
                                        )
                                )
                        )
        );

    }
}
