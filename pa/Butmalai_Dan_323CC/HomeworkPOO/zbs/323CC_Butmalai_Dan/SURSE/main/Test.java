package main;

import main.Store.*;
import main.Department.*;

import javax.swing.*;
import java.awt.*;
import java.util.Vector;


public class Test {
    /*
        Classa test e folosita pentru a verifica functionalitatile implementate si a invoga interfata grafica
        in cazul in care programul nu e rulat in linia de comanda.
    */
    static Test instance = null;
    private static Event eventUI;

    public static void print(Object mssg){
        System.out.println(mssg);
    }

    public void parseCostumers(Vector<String> customers){
        Store store = Store.getInstance();
        for(int i = 1; i < customers.size(); i++){
            store.enter(new Customer(customers.get(i).split(";")));
        }

    }

    private Test(){

    }

    public void parseStore(Vector<String> store_data){
        Store store = Store.getInstance();
        store.setName(store_data.get(0));
        Builder dep_factory = new Builder();
        for(int i = 1; i < store_data.size(); i++){
            Department dep = dep_factory.getNewDep(store_data.get(i));
            store.addDepartment(dep);
            int k = i;
            for(int j = k+2; j < k + 2 + Integer.valueOf(store_data.get(k+1)); j++){
                Item item = new Item(store_data.get(j).split(";"));
                Store.getInstance().itemTreeMap.put(item.getId(),item);
                dep.addItem(item);
                store.addItem(item);
                i = j;
            }
        }
    }

    public Event parseEvents(Vector<String> events){
        Event event_solver = new Event();
        for (int i =1 ; i < events.size(); i++){
            String event = events.get(i);
            event_solver.solve(event);
        }
        return event_solver;
    }
    public static void main(String args[]){

        boolean testIDE = false;

        Test test = Test.getInstance();

        if(args.length != 0){
            DataRead read = new DataRead("");

            test.parseCostumers(read.getData(args[0]));
            test.parseStore(read.getData(args[1]));
            Event events_solved = test.parseEvents(read.getData(args[2]));

            Vector<String> officialResult = read.getData(args[3]);
            Vector<String> result = events_solved.getResult();

            for (int j = 0 ; j < result.size(); j++){
                System.out.println(result.get(j));
            }

            Store.getInstance().clear();

        }else
            testIDE = true;

        String executionPath = System.getProperty("user.dir");
        if(System.getProperty("os.name").contains("inux")){
            executionPath += "/main/inputs";
        }else{
            executionPath += "\\src\\main\\inputs";
        }

        if(testIDE){
            DataRead read = new DataRead(executionPath);
            for(int i = 0; i <= 9; i++){
                System.out.println(i);
                String test_folder = "0" + String.valueOf(i);
                test.parseCostumers(read.getData(test_folder,"cust"));
                test.parseStore(read.getData(test_folder, "store"));
                Event events_solved = test.parseEvents(read.getData(test_folder, "eve"));

                Vector<String> officialResult = read.getData(test_folder, "result");
                Vector<String> result = events_solved.getResult();

                for (int j = 0 ; j < result.size(); j++){
                    System.out.println(result.get(j));
                }

                Store.getInstance().clear();
            }
        }
        if(args.length == 0) {
            if (GraphicsEnvironment.isHeadless()) {
                System.out.println("\n\n!!!! GUI NOT AVAILABLE !!!!\n\n");
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        new UImain();
                    }
                });
            }

        }

    }

    public static Test getInstance() {
        if(instance == null) {
            eventUI = new Event();
            return new Test();
        }
        else return instance;
    }

    public Event getEventUI() {
        return eventUI;
    }
}
