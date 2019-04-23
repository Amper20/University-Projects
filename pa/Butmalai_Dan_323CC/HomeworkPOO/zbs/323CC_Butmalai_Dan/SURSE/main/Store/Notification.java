package main.Store;

public class Notification {
    //Implementeaza notificarile primite de fiecare client
    private int display = -1;
    private int departmentID, itemID = -1, timeline;
    private String message = "";
    public enum NotificationType {ADD, REMOVE, MODIFY}
    private Item modified;
    NotificationType type;


    public Notification(NotificationType type, Item item, int departmentID, int display, int ItemId){
        this.display = display;
        this.itemID = ItemId;
        this.type = type;
        modified = item;
        if(item != null)
            itemID = item.getId();
        this.departmentID = departmentID;
    }

    public boolean display(){
        return display > 0;
    }

    public void setModified(Item modified) {
        this.modified = modified;
    }

    public Item getModified() {
        return modified;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public void setTimeline(int timeline) {
        this.timeline = timeline;
    }

    public void setDepartmentID(int departmentID) {
        this.departmentID = departmentID;
    }

    public void setItemID(int itemID) {
        this.itemID = itemID;
    }

    public void setType(NotificationType type) {
        this.type = type;
    }

    public int getTimeline() {
        return timeline;
    }

    public int getItemID() {
        return itemID;
    }

    public int getDepartmentID() {
        return departmentID;
    }

    public NotificationType getType() {
        return type;
    }

    @Override
    public String toString() {
        String ret = "";
        if(type == NotificationType.ADD)
            ret += "ADD;";
        else {
            if (type == NotificationType.REMOVE)
                ret += "REMOVE;";
            else
                ret += "MODIFY;";
        }
        ret += String.valueOf(itemID) + ";";
        ret += String.valueOf(departmentID);
        return ret;
    }
}
