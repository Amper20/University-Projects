package main.Fragments;

import main.Store.Item;
import main.Store.Store;
import main.Test;
import main.UImain;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.Vector;

public class ItemsList extends Fragment {
    /*
        ItemaList e un Fragment care e folosit la vizualizarea si editarea paginei cu lista de itemuri
     */
    DefaultListModel<String> list;
    JList<String> dispList;
    JScrollPane listPane;

    GroupLayout layout;
    UImain callback;

    JButton backButton = new JButton("Back");
    JButton sortName, sortCheap, sortExpensive,deleteSelected;
    ActionListener onSortName, onSortCheap, onSortExpensive, onDeleteSelected;


    public ItemsList(int ID, UImain callback) {

        super(ID, callback);
        this.callback = callback;
        backButton.addActionListener(callback);

        list = new DefaultListModel<>();
        updateList();

        listPane = new JScrollPane();
        dispList = new JList<>(list);
        listPane.setViewportView(dispList);

        sortCheap = new JButton("Sort Cheap First");
        sortExpensive = new JButton("Sort Expensive First");
        sortName = new JButton("Sort Alphabetical");
        deleteSelected = new JButton("Delete Selected");

        setLayout();

        setListeners();

    }

    private void setLayout() {

        layout = new GroupLayout(getPanel());
        layout.setAutoCreateContainerGaps(true);
        layout.setAutoCreateGaps(true);

        layout.setHorizontalGroup(
                layout.createParallelGroup()
                        .addGroup(layout.createSequentialGroup()
                                .addComponent(backButton)
                                .addComponent(sortCheap)
                                .addComponent(sortExpensive)
                                .addComponent(sortName)
                                .addComponent(deleteSelected))
                        .addComponent(listPane)

        );
        layout.setVerticalGroup(
                layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup()
                                .addComponent(backButton)
                                .addComponent(sortCheap)
                                .addComponent(sortExpensive)
                                .addComponent(sortName)
                                .addComponent(deleteSelected)
                        )
                        .addComponent(listPane)
        );
        getPanel().setLayout(layout);
    }

    private void setListeners(){
        onDeleteSelected = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                resetButtons();
                int indx = dispList.getSelectedIndex();
                if(dispList.getSelectedIndex() != -1){
                    deleteSelected.setBackground(new Color(0,255,0));
                    Vector<Item> itms =  Store.getInstance().getItems();
                    Test.getInstance().getEventUI().solve("delProduct;" + String.valueOf(itms.get(indx).getId()));
                }else
                    deleteSelected.setBackground(new Color(255,0,0));
                updateList();
            }
        };
        onSortName = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                resetButtons();
                Vector<Item> data = new Vector<>();
                for(Item itm : Store.getInstance().getItems())
                    data.add(new Item(itm));
                Comparator<Item> cmp = new Comparator<Item>() {
                    @Override
                    public int compare(Item o1, Item o2) {
                        return o1.getName().compareTo(o2.getName());
                    }
                };
                updateList(data);
            }
        };
        onSortCheap = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                resetButtons();
                Vector<Item> data = new Vector<>();
                for(Item itm : Store.getInstance().getItems())
                    data.add(new Item(itm));
                Comparator<Item> cmp = new Comparator<Item>() {
                    @Override
                    public int compare(Item o1, Item o2) {
                        if(o1.getPrice() > o2.getPrice())
                            return 1;
                        return -1;
                    }
                };
                data.sort(cmp);
                updateList(data);
            }
        };
        onSortExpensive = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                resetButtons();
                Vector<Item> data = new Vector<>();
                for(Item itm : Store.getInstance().getItems())
                    data.add(new Item(itm));
                Comparator<Item> cmp = new Comparator<Item>() {
                    @Override
                    public int compare(Item o1, Item o2) {
                        if(o1.getPrice() < o2.getPrice())
                            return 1;
                        return -1;
                    }
                };
                data.sort(cmp);
                updateList(data);
            }
        };

        sortExpensive.addActionListener(onSortExpensive);
        sortCheap.addActionListener(onSortCheap);
        deleteSelected.addActionListener(onDeleteSelected);
        sortName.addActionListener(onSortName);
    }

    public void resetButtons(){
        sortName.setBackground(new JButton().getBackground());
        sortCheap.setBackground(new JButton().getBackground());
        sortExpensive.setBackground(new JButton().getBackground());
        deleteSelected.setBackground(new JButton().getBackground());;
    }

    public void updateList(){
        list.clear();
        for(Item itm : Store.getInstance().getItems())
            list.addElement(itm.toString());
    }
    public void updateList(Vector<Item> data){
        list.clear();
        for(Item itm : data)
            list.addElement(itm.toString());
    }
}
