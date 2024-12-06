#include <string>

struct Prototype 
{
    virtual ~Prototype() {};
    virtual Prototype* clone() const = 0;
};

class Character : Prototype 
{   
public:
    Character(size_t initHealth, size_t initMana, const std::string& initName) :
        health(initHealth), mana(initMana), name(initName)
    {
        // какая-то своя сложная логика по созданию персонажа
        // можеть быть это будет работа с файловой системой 
        // и загрузкой моделек, записи речи персонажа и тд
        // или сложный запрос к базе данных для загрузки необходимых данных
        // в общем случае высоконагруженная работа 
    }
    ~Character() {} 
    Character* clone() const override
    {
        return new Character(*this);
    }

    // другие методы персонажа
    // такие как setNewName, setNewMana, setNewHealth
    // и другие для того чтобы работать уже с клонированным персонажем
private:
    size_t health;
    size_t mana;
    std::string name;

};

int main()
{
    Character* character = new Character(590, 440, "Warrior");
    Character* clonedCharacter = character->clone();
    // работа с персонажем
    // и взаимодействие с игрой

    delete character;
    delete clonedCharacter;

    return 0;
}


