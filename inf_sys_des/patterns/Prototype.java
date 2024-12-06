public interface Sheep {
    Sheep clone();
    String getName();
    void setName(String name);
}

public class BlackSheep implements Sheep {
    private String name;

    public BlackSheep(String name) {
        this.name = name;
    }

    @Override
    public Sheep clone() {
        return new BlackSheep(this.name);
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public void setName(String name) {
        this.name = name;
    }
}

public class Main {
   public static void main(String[] args) {

       Sheep blackPrototype = new BlackSheep("Baa Baa");
       Sheep clonedBlackSheep = blackPrototype.clone();

       clonedBlackSheep.setName("Midnight");
       System.out.println("Black sheep: " + clonedBlackSheep.getName());
   }
} 
