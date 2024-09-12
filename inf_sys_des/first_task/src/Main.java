import java.util.ArrayList;
import java.util.Scanner;
import java.util.Iterator;


public class Main
{
    final static int MANY_WEEDS = 2000; // Количество растений в пруду
    final static double WEED_SIZE = 15; // Начальный размер растения
    final static double WEED_RATE = 2.5; // Скорость роста растения
    final static int INIT_FISH = 300; // Начальный размер популяции рыб
    final static double FISH_SIZE = 50; // Размер рыб
    final static double FRACTION = 0.5; // Рыба каждую неделю должна съедать объем равный FRACTION * FISH_SIZE
    final static int AVERAGE_NIBBLES = 30; //Среднее количество растений, частично съедаемых рыбами в течение недели равно
    // произведению AVERAGE_NIBBLES на размер популяции рыб
    final static double BIRTH_RATE = 0.05; // Общее количество новых рыб равно произведению размера популяции на константу BIRTH_RATE


    public static void pondWeek(ArrayList<Herbivore> fish, ArrayList<Plant> weeds)
    {
        int manyIterations = AVERAGE_NIBBLES * fish.size( );
        for (int i = 0; i < manyIterations; ++i)
        {
            //Случайный выбор рыбы и растения для питания
            Herbivore herbivore = fish.get(getRand(fish.size()));
            Plant plant = weeds.get(getRand(weeds.size()));

            herbivore.nibble(plant);
        }

        //Моделирование одной недели для растений
        for (Plant tempPlant : weeds) {
            tempPlant.simulateWeek();
        }

        //Моделирование одной недели для рыб с удалением из популяции умерших рыб
        for (Iterator<Herbivore> herbivoreIterator = fish.iterator(); herbivoreIterator.hasNext();)
        {
            Herbivore tempHerbivore = herbivoreIterator.next();
            tempHerbivore.simulateWeek();

            if (!tempHerbivore.isAlive()) herbivoreIterator.remove();
        }

        //Добавление в популяцию новых рыб
        int newFishPopulation = (int)(BIRTH_RATE * fish.size());
        for (int i = 0; i < newFishPopulation; i++)
            fish.add(new Herbivore(0, FISH_SIZE, FISH_SIZE * FRACTION));
    }

    public static double totalMass(ArrayList<Plant> weeds)
    {
        double total = 0.0;
        for( Plant p : weeds ) total += p.getSize();

        return total;
    }

    private static int getRand(int n)
    {
        return (int) (0.0 + Math.random() * n);
    }


    public static void main(String[] args)
    {
        ArrayList<Plant> weeds = new ArrayList<>(MANY_WEEDS);
        ArrayList<Herbivore> fish = new ArrayList<>(INIT_FISH);

        Scanner in = new Scanner(System.in);
        int manyWeeks;   // Количество недель для моделирования
        int i;

        System.out.println("How many weeks shall I simulate? ");
        manyWeeks=in.nextInt();

        //Создание популяции травоядных рыб
        for (i = 0; i < INIT_FISH; i++) fish.add(new Herbivore (0, FISH_SIZE, FISH_SIZE * FRACTION));

        //Создание популяции растений
        for (i = 0; i < MANY_WEEDS; i++) weeds.add(new Plant(WEED_RATE, WEED_SIZE));

        //Моделирование жизни в пруду
        for (i = 0; i < manyWeeks; i++)
        {
            pondWeek(fish, weeds);
            System.out.println((i + 1) + "\t" +fish.size() + "\t" +totalMass(weeds));
        }
    }
}