package main.Department;

public class Builder {
    /*
        Clasa Builder e folosita pentru a crea un departament nou
     */
    public Department getNewDep(String name){
        if(name.contains("Music")){
            return new MusicDepartment(name);
        }
        if(name.contains("Video")){
            return new VideoDepartment(name);
        }
        if(name.contains("Book")){
            return new BookDepartment(name);
        }
        if(name.contains("Soft")){
            return new SoftwareDepartment(name);
        }
        return null;
    }
}
