package main;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FilenameFilter;
import java.util.Arrays;
import java.util.Scanner;
import java.util.TreeMap;
import java.util.Vector;

public class DataRead {
    //Parseaza datele de intrare
    String path = "";
    String[] paths;
    TreeMap<String, Vector<String>> tests;
    public DataRead(String path){
        this.path = path;
        tests = new TreeMap<String, Vector<String>>();
        getDirectores();
    }
    public void getDirectores(){
        File directories = new File(path);
        paths = directories.list(new FilenameFilter() {
            @Override
            public boolean accept(File current, String name) {
                return new File(current, name).isDirectory();
            }
        });
        if (paths != null) {
            for(int i = 0 ; i < paths.length; i++){
                if(System.getProperty("os.name").contains("inux")){
                    paths[i] = path + "/" + paths[i];
                }else{
                    paths[i] = path + "\\" + paths[i];
                }
                File folder = new File(paths[i]);
                String[] files  = folder.list(new FilenameFilter() {
                    @Override
                    public boolean accept(File dir, String name) {
                        return new File(dir,name).isFile();
                    }
                });
                Vector<String> value  = new Vector<>();
                if (files != null) {
                    value.addAll(Arrays.asList(files));
                }
                tests.put(paths[i], value);
            }
        }
    }

    public Vector<String> getData(String test, String key){
        Vector<String> ret = new Vector<>();
        for(String k : tests.keySet()){
            for(String val : tests.get(k)){
                if(k.contains(test) && val.contains(key)){
                    File file;
                    if(System.getProperty("os.name").contains("inux")){
                        file = new File(k + "/" + val);
                    }else{
                        file = new File(k + "\\" + val);
                    }
                    Scanner sc = null;
                    try {
                        sc = new Scanner(file);
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    }
                    if (sc != null) {
                        while (sc.hasNextLine())
                            ret.add(sc.nextLine());
                    }
                }
            }
        }
        return ret;
    }

    public Vector<String> getData(String filePath){
        Vector<String> ret = new Vector<>();

        File file = new File(filePath);
        Scanner sc = null;
        try {
            sc = new Scanner(file);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        if (sc != null) {
            while (sc.hasNextLine())
                ret.add(sc.nextLine());
        }

        return ret;
    }

    public Vector<String> getData(){
        Vector<String> ret = new Vector<>();
        File file = new File(path);
        Scanner sc = null;
        try {
            sc = new Scanner(file);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        if (sc != null)
            while (sc.hasNextLine())
                ret.add(sc.nextLine());
        return ret;
    }
}
