package main.Store;

public class Item {
    //implementeaza conceptul de itme
    private String name;
    private int id;
    private double price;

    public Item(String name, int id, double price){
        this.name = name;
        this.id = id;
        this.price = price;
    }

    public Item(Item item){
        name = item.getName();
        id = item.getId();
        price= item.getPrice();
    }

    public Item(String[] data){
        this(data[0], Integer.valueOf(data[1]), Double.valueOf(data[2]));
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public void setName(String name){
        this.name = name;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    @Override
    public String toString() {
        return name + ";" + String.valueOf(id) + ";" + String.valueOf(price);
    }
}
