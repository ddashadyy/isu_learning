#include <iostream>
#include <string>
#include <memory>
#include <vector>

class Organism;
class Animal;
class Grass;
class VenusFlyTrap;
class Giraffe;
class Bear;
class Lion;
class Bird;
class HummingBird;
class Eagle;
struct IHerbivore;
struct IPredator;

class Organism
{
public:
    Organism() = default;
    Organism( const std::string& name ) : speciesName( name ) {};
    virtual ~Organism() = default;

    virtual std::string getSpeciesName() const = 0;

protected:
    std::string speciesName{};
};

struct IHerbivore
{
    virtual void eat( Grass& g ) const = 0;
};

struct IPredator
{
    virtual void eat( Animal& g ) const = 0;
};


class Animal : public Organism
{
public:
    Animal() = default;
    Animal( const std::string& name ) { speciesName = name; }
    virtual ~Animal() = default;

    virtual void sound() const = 0;
    
};

class Grass : public Organism
{
public:
    Grass() = default;
    Grass( const std::string& name ) { speciesName = name; }
    virtual ~Grass() = default;

    virtual std::string getSpeciesName() const override { return speciesName; }
};

class VenusFlyTrap : public Grass, IPredator
{
public:
    VenusFlyTrap() = default;
    VenusFlyTrap( const std::string& name ) { speciesName = name; }
    ~VenusFlyTrap() = default;

    void eat( Animal& a ) const override { std::cout << "I am " << getSpeciesName() << " eating Animal\n"; }
    std::string getSpeciesName() const override { return speciesName; }
};

class Giraffe : public Animal, IHerbivore
{
public:
    Giraffe() = default;
    Giraffe( const std::string& name) { speciesName = name; }
    ~Giraffe() = default;
    
    std::string getSpeciesName() const override { return speciesName; }
    void sound() const override { std::cout << "I am Giraffe\n"; }
    void eat( Grass& g ) const override { std::cout << "I am " << getSpeciesName() << " eating Grass\n"; }
};

class Bear : public Animal, IHerbivore
{
public:
    Bear() = default;
    Bear( const std::string& name ) { speciesName = name; }
    ~Bear() = default;

    std::string getSpeciesName() const override { return speciesName; }
    void sound() const override { std::cout << "I am Bear\n"; }
    void eat( Grass& g ) const override { std::cout << "I am " << getSpeciesName() << " eating Grass\n"; }

};

class Lion : public Animal, IPredator
{
public:
    Lion() = default;
    Lion( const std::string& name ) { speciesName = name; }
    ~Lion() = default;

    std::string getSpeciesName() const override { return speciesName; }
    void sound() const override { std::cout << "I am Lion\n"; }
    void eat( Animal& a ) const override { std::cout << "I am " << getSpeciesName() << " eating Animal\n"; }
};

class Bird : public Animal
{
public:
    Bird() = default;
    Bird( const std::string& name ) { speciesName = name; }
    virtual ~Bird() = default;

    virtual std::string getSpeciesName() const override { return speciesName; }
    virtual void sing() const = 0;
    virtual void fly() const = 0;
};

class HummingBird : public Bird, IHerbivore
{
public:
    HummingBird() = default;
    HummingBird( const std::string& name ) { speciesName = name; }
    ~HummingBird() = default;

    std::string getSpeciesName() const override { return speciesName; }
    void sing() const override { std::cout << "I am singing\n"; };
    void fly() const override { std::cout << "I am flying\n"; };
    void eat( Grass& g ) const override { std::cout << "I am " << getSpeciesName() << " eating Grass\n"; };
    void sound() const override { std::cout << "I am HummingBird\n"; }
};

class Eagle : public Bird, IPredator
{
public:
    Eagle() = default;
    Eagle( const std::string& name ) { speciesName = name; }
    ~Eagle() = default;

    std::string getSpeciesName() const override { return speciesName; }
    void sing() const override { std::cout << "I am singing\n"; };
    void fly() const override { std::cout << "I am flying\n"; };
    void eat( Animal& g ) const override { std::cout << "I am " << getSpeciesName() << " eating Aniaml\n"; };
    void sound() const override { std::cout << "I am Eagle\n"; }
};




int main()
{
    std::vector<std::shared_ptr<Animal>> animals = {
        std::make_shared<Giraffe>("Giraffe"),
        std::make_shared<Bear>("Bear"),
        std::make_shared<Lion>("Giraffe"),
        std::make_shared<HummingBird>("HummingBird"),
        std::make_shared<Eagle>("Eagle")
    };
    
    for (const auto& animal : animals)
        animal->sound();

    auto venusFlyTrap_ptr = std::make_unique<VenusFlyTrap>("VenusFlyTrap");
    auto grass_ptr = std::make_unique<Grass>("Grass");

    

    return 0;
}