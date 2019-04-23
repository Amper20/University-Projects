package main.Store;

import java.util.Collection;
import java.util.Comparator;
import java.util.ListIterator;

public class ItemList{
    /*
        Implementeaza o lista dublu inlantuita
     */
    Node<Item> head = null;
    int time = 0;
    int size = 0;

    public int getSize() {
        return size;
    }

    public void setSize(int size) {
        this.size = size;
    }

    private Comparator<Node<Item>> cmp = new Comparator<Node<Item>>() {
        @Override
        public int compare(Node<Item> o1, Node<Item> o2) {
            if(o1.getItem().getPrice() == o2.getItem().getPrice())
                return o2.getItem().getName().compareTo(o1.getItem().getName());
            return (o1.getItem().getPrice() < o2.getItem().getPrice() ? 1 : 0);
        }
    };

    public void sort(Node<Item> head){
        for(ItemIterator<Item> i = new ItemIterator<>(head); i.hasNext(); i.next()){
            for (ItemIterator<Item> j = new ItemIterator<>(head); j.hasNext(); j.next()){
                int timeI = i.getTime();
                int timeJ = j.getTime();
                Item I = i.getItem();
                Item J = j.getItem();
                if(cmp.compare(i.getCurrent(),j.getCurrent()) > 0){
                    (i.getCurrent()).setItem(J);
                    i.setTime(timeJ);
                    (j.getCurrent()).setItem(I);
                    j.setTime(timeI);
                }
            }
        }
    }

    public boolean add( Item element ){
        Node<Item> curr = new Node<>(element, time);
        time++;
        size++;
        if(head == null){
            head = curr;
            return true;
        }
        curr.next = head;
        head.prev = curr;
        head = curr;
        sort(head);
        return true;
    };

    public boolean addAll(Collection<? extends Item> collection){
        for (Item it : collection){
            this.add(it);
            size++;
        }
        return true;
    }

    public Item getItem(int index){
        if(getNode(index) != null)
            return getNode(index).getItem();
        return null;
    }

    public Node<Item> getNode(int index){
        for(ItemIterator<Item> it = new ItemIterator<>(head);it.hasNext(); it.next(),index--){
            if(index == 0)
                return it.getCurrent();
        }
        return null;
    }

    public int indexOf(Item item) {
        int index = 0;
        for(ItemIterator<Item> it = new ItemIterator<>(head);it.hasNext(); it.next(),index++) {
            if(it.getItem().getId() == item.getId())
                return index;
        }
        return -1;
    }

    public int indexOf(Node<Item> node){
        int index = 0;
        for(ItemIterator<Item> it = new ItemIterator<>(head);it.hasNext(); it.next(),index++) {
            if(it.getCurrent() == node)
                return index;
        }
        return -1;
    }

    public boolean contains(Node<Item> node){
        return indexOf(node) != -1;
    }

    public boolean contains(Item item){
        return indexOf(item) != -1;
    }

    public Item remove(int index){
        if(head == null)
            return null;
        if(index == 0){
            Item hd = head.getItem();
            if(head!= null){
                head = head.next;
                if(head != null)
                    head.prev = null;
            }
            size--;
            return hd;
        }

        ItemIterator<Item> curr = new ItemIterator<>(head);

        for( ;curr.hasNext() && (index != 0); curr.next(),index--);

        if (curr.hasPrevious() && curr.getPrev() != null) {
            Node<Item> prv = curr.getPrev();
            Node<Item> crr = curr.getCurrent();
            prv.next = crr.next;
            crr.prev = prv;
            size--;
            return curr.getItem();
        }
        return null;
    }

    public boolean remove(Item item){
        return remove(indexOf(item)) != null;
    }

    public boolean removeAll(Collection<? extends Item> collection){
        boolean ret = true;
        for (Item it : collection )
            ret &= remove(it);
        return ret;
    }

    public boolean isEmpty(){
        return head == null;
    }

    public ListIterator<Item> ListIterator(int index) {
        ItemIterator<Item> it = new ItemIterator<>(head);
        for (;it.hasNext() && index != 0; it.next(),index--);
        return it;
    }

    public ListIterator<Item> ListIterator(){
        return ListIterator(0);
    }

    public Double getTotalPrice(){
        double ret = 0.0;
        for(ItemIterator<Item> it = new ItemIterator<>(head); it.hasNext(); it.next()) {
            ret += it.getItem().getPrice();
        }
        return ret;
    }

    public Node<Item> getHead() {
        return head;
    }

    public static class Node<K>{

        K item;
        int timeline = 0;
        Node<K> next = null, prev = null;

        Node(K item, int timeline){
            this.item = item;
            this.timeline = timeline;
            next = null;
        }

        public int getTimeline() {
            return timeline;
        }

        public void setTimeline(int timeline) {
            this.timeline = timeline;
        }

        public void setItem(K item){
            this.item = item;
        }

        public K getItem() {
            return item;
        }

        @Override
        public String toString() {
            return item.toString();
        }
    }

    public static class ItemIterator<K> implements ListIterator<K> {
        Node<K> current = null;
        int index = 0;

        public ItemIterator(Node<K> head){
            this.current = head;
        }

        public Node<K> getPrev(){
            if(current == null)
                return null;
            return current.prev;
        }

        public Node<K> getCurrent(){
            return current;
        }

        public Node<K> getNext(){
            if(current == null)
                return null;
            return current.next;
        }

        public boolean hasNext(){
            return current != null;
        }

        public K next(){
            if(this.hasNext()) {
                index++;
                current = current.next;
                if(current != null)
                    return current.getItem();
            }
            return null;
        }

        @Override
        public boolean hasPrevious() {
            return current != null;
        }

        @Override
        public K previous() {
            if(hasPrevious()) {
                index--;
                current = current.prev;
                return current.prev.getItem();
            }
            return null;
        }

        @Override
        public int nextIndex() {
            if(current == null)
                return -1;
            if(current.next != null)
                return index+1;
            return -1;
        }

        @Override
        public int previousIndex() {
            if(current == null)
                return -1;
            if(current.prev != null)
                return index-1;
            return -1;
        }

        @Override
        public void remove() {

        }

        @Override
        public void set(K Node) {

        }

        @Override
        public void add(K Node) {

        }

        public K getItem(){
            return current.getItem();
        }

        public int getTime(){
            return current.getTimeline();
        }
        public void setTime(int time){
            current.setTimeline(time);
        }
    }

    @Override
    public String toString() {
        StringBuilder ret = new StringBuilder();
        for(ItemIterator<Item> it = new ItemIterator<>(head); it.hasNext(); it.next())
            ret.append(it.getItem().toString());
        return ret.toString();
    }
}
