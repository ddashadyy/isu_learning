//
//
//  Generated by StarUML(tm) Java Add-In
//
//
//


/**
 * Класс, моделирующий поведение растения
 **/
public class Plant extends Organism {
    /**
     * Конструктор класса. Поведение по умолчанию - вызов конструктора родителя
     *
     * @param initRate
     * @param initSize
     **/
    Plant(double initRate, double initSize) {
        super(initRate, initSize);
    }

    /**
     * Функция моделирует процесс питания данным растением.
     * Питание растением заключается в вызове метода изменения размера растения с отрицательным значением amount
     *
     * @param amount
     **/
    public void nibbledOn(double amount) {
        super.alterSize(-amount);
    }
}
