//
//
//  Generated by StarUML(tm) Java Add-In
//
//
//


/**
 * Моделирование поведения травоядной рыбы
 **/
public class Herbivore extends Animal {
    /**
     * Вызывает конструктор родителя.
     *
     * @param initRate
     * @param initSize
     * @param initNeed
     **/
    Herbivore(double initRate, double initSize, double initNeed) {
        super(initRate, initSize, initNeed);
    }

    /**
     * Моделирование питания рыбы растением meal.
     * Параметры питания задаются в модели. Сначала по параметрам модели вычисляется количество пищи,
     * которое может быть съедено одной рыбой, затем это количество обрабатывается текущей как съеденное и
     * растению сообщается, что им питались и откусили некоторое количество.
     *
     * @param meal
     **/
    public void nibble(Plant meal) {
        double eatSize = Math.min(Math.min(getNeed() * 0.1, meal.getSize() / 2), stillNeed());
        meal.nibbledOn(eatSize);
        eat(eatSize);
    }
}
