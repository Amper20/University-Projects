package main.Fragments;

import main.*;
import main.Test;
import main.UImain;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Loader extends Fragment{
    /*
        Pagina principala
    */
    JTextField storeDataPath;
    JTextField customersDataPath;
    JButton storeLoad, customerLoad;
    ActionListener onStoreLoad, onCustomerLoad, onViewITems, onViewCustomers;
    JButton viewItems,viewCustomers;
    UImain callback;
    boolean viewI = false, viewC = false;
    public Loader(int ID, UImain callback) {
        super(ID, callback);

        getPanel().setLayout(new GridLayout(3,2));

        this.callback = callback;

        storeDataPath = new JTextField(30);
        storeDataPath.setText("C:\\Users\\Master\\Desktop\\HomeworkPOO\\src\\main\\inputs\\test03\\store.txt");
        customersDataPath = new JTextField(30);
        customersDataPath.setText("C:\\Users\\Master\\Desktop\\HomeworkPOO\\src\\main\\inputs\\test03\\customers.txt");
        storeLoad = new JButton("Load Store");
        customerLoad = new JButton("Load Customers");
        viewItems = new JButton("View Items");
        viewItems.setEnabled(false);
        viewCustomers = new JButton("View Customers");
        viewCustomers.setEnabled(false);

        addListeners();

        getPanel().add(storeDataPath);
        getPanel().add(storeLoad);

        getPanel().add(customersDataPath);
        getPanel().add(customerLoad);

        getPanel().add(viewItems);
        getPanel().add(viewCustomers);

    }

    private void addListeners() {
        onStoreLoad = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String path = storeDataPath.getText();
                Test instance = Test.getInstance();
                DataRead read = new DataRead(path);
                storeLoad.setEnabled(false);
                instance.parseStore(read.getData());
                viewC = true;
                if(viewI) {
                    viewItems.setEnabled(true);
                    viewCustomers.setEnabled(true);
                }
            }
        };

        onCustomerLoad = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String path = customersDataPath.getText();
                Test instance = Test.getInstance();
                DataRead read = new DataRead(path);
                instance.parseCostumers(read.getData());
                customerLoad.setEnabled(false);
                viewI = true;
                if(viewC) {
                    viewItems.setEnabled(true);
                    viewCustomers.setEnabled(true);
                }
            }
        };

        onViewCustomers = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ((CustomerOverview)callback.getFragment(5)).updateList();
                callback.changeFragment(callback.getFragment(5));
            }
        };

        onViewITems = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                callback.changeFragment(callback.getFragment(2));
            }
        };

        storeLoad.addActionListener(onStoreLoad);
        customerLoad.addActionListener(onCustomerLoad);
        viewItems.addActionListener(onViewITems);
        viewCustomers.addActionListener(onViewCustomers);
    }

}
