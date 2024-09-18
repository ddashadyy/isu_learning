public class Predator extends Animal {

    Predator(double initRate, double initSize, double initNeed) {
        super(initRate, initSize, initNeed);
    }

    public void chase(Animal prey, double chance) {
        double hunt = Math.random();
        if ((hunt <= chance) && (stillNeed() > 0)) {
            eat(prey.getSize());
            prey.death();
        }
    }
}
