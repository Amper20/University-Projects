package main.Store;

public interface Observer {
    //interfata folosita in implementarea paternului Subject-Observer
    abstract void update(Notification message);
}
