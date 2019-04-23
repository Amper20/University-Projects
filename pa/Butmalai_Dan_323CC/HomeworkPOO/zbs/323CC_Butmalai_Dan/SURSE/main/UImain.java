package main;

import main.Fragments.Fragment;

import java.awt.event.*;
import java.util.Stack;
import java.util.TreeMap;
import java.util.Vector;
import javax.swing.*;

public class UImain implements ActionListener {
    /*
        Classa implementeaza Conceptul de Stack viewport
        Contine un stack "istoric" de fragmenturi vizitate
        La apasarea butonului Back stack si fragmentul din viewport e actualizat
     */
    private JFrame frame;
    private Vector<Fragment> fragments;
    private Stack<Fragment> stack;
    int [] panelsID = new int[]{0,1,2,3,4,5,6,7};
    private TreeMap<Integer, Fragment> fragmentMap;

    public UImain() {
        frame = new JFrame("STORE");
        fragments = new Vector<>();
        stack = new Stack<>();
        fragmentMap = new TreeMap<>();

        initializePanels();

        changeFragment(fragments.get(0));
    }

    private void initializePanels() {
        for(int i = 0 ; i < panelsID.length; i++) {
            Fragment current = Fragment.getNewFragment(i, this);
            fragmentMap.put(i, current);
            fragments.add(current);
        }
    }

    public void actionPerformed(ActionEvent backEvent) {
        JButton button = (JButton) backEvent.getSource();
        frame.remove(stack.pop().getPanel());
        frame.setContentPane(stack.peek().getPanel());
        frame.validate();
        frame.repaint();
    }
    public void changeFragment(Fragment nextFragment){
        stack.push(nextFragment);
        frame.setContentPane(stack.peek().getPanel());
        frame.setSize(800, 500);
        frame.setVisible(true);
        frame.validate();
        frame.repaint();
    }
    public Fragment getFragment(int ID){
        return fragmentMap.get(ID);
    }
}