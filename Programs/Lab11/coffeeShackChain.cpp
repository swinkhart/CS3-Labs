// coffee shack decorator and chain of responsibility
// Seth Winkhart
// 4/17/2022

#include <string>
#include <iostream>

enum class DrinkType
{
    small,
    medium,
    large
};

// component
class Drink
{
public:
    Drink(DrinkType type = DrinkType::small, int price = 0) : type_(type), price_(price) {}
    virtual int getPrice() const { return price_; }
    virtual std::string getName() const;

private:
    int price_;
    DrinkType type_;
};

// decorator pattern stuff
class Sugar : public Drink
{
public:
    Sugar(const Drink *wrapped) : wrapped_(wrapped) {}
    int getPrice() const { return wrapped_->getPrice() + 1; }
    std::string getName() const { return wrapped_->getName() + "sugar, "; }

private:
    const Drink *wrapped_;
};

class Cream : public Drink
{
public:
    Cream(const Drink *wrapped) : wrapped_(wrapped) {}
    int getPrice() const { return wrapped_->getPrice() + 2; }
    std::string getName() const { return wrapped_->getName() + "cream, "; }

private:
    const Drink *wrapped_;
};

class MilkFoam : public Drink
{
public:
    MilkFoam(const Drink *wrapped) : wrapped_(wrapped) {}
    int getPrice() const { return wrapped_->getPrice() + 3; }
    std::string getName() const { return wrapped_->getName() + "milk foam, "; }

private:
    const Drink *wrapped_;
};

// chain of responsibility pattern stuff
// administrator
class Barista
{
public:
    Barista(Barista *successor = nullptr) : successor_(successor) {}
    virtual Drink *handleRequest(char choice, Drink *drink) { return successor_->handleRequest(choice, drink); }

private:
    Barista *successor_;
};

// derived chain on responsibility classes
class JuniorBarista : public Barista
{
public:
    JuniorBarista(Barista *successor = nullptr) : Barista(successor) {}
    Drink *handleRequest(char choice, Drink *drink) override;
};

class SeniorBarista : public Barista
{
public:
    SeniorBarista(Barista *successor = nullptr) : Barista(successor) {}
    Drink *handleRequest(char choice, Drink *drink) override;
};

class Manager : public Barista
{
public:
    Manager(Barista *successor = nullptr) : Barista(successor) {}
    Drink *handleRequest(char choice, Drink *drink) override;
};

// member functions
std::string
Drink::getName() const
{
    std::string size;
    if (type_ == DrinkType::small)
    {
        size = "small";
    }
    else if (type_ == DrinkType::medium)
    {
        size = "medium";
    }
    else
    {
        size = "large";
    }

    return size + " coffee with ";
}

Drink *JuniorBarista::handleRequest(char choice, Drink *drink)
{
    std::cout << "Welcome to Ripoff Shack, can I get you [l]arge, [m]edium, or [s]mall coffee?: ";
    std::cin >> choice;

    DrinkType type;
    int price;
    switch (choice)
    {
    case 's':
        type = DrinkType::small;
        price = 5;
        break;
    case 'm':
        type = DrinkType::medium;
        price = 7;
        break;
    case 'l':
        type = DrinkType::large;
        price = 10;
        break;
    }

    drink = new Drink(type, price);

    std::cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one?: ";
    std::cin >> choice;

    if (choice == 'd')
    {
        std::cout << "Junior barista prepares the coffee." << std::endl;
        return drink;
    }
    else
    {
        return Barista::handleRequest(choice, drink);
    }
}

Drink *SeniorBarista::handleRequest(char choice, Drink *drink)
{
    while (choice != 'f')
    {
        std::cout << "Senior Barista prepares the coffee." << std::endl;

        switch (choice)
        {
        case 's':
            drink = new Sugar(drink);
            break;
        case 'c':
            drink = new Cream(drink);
            break;
        case 'd':
            return drink;
            break;
        }

        std::cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one?: ";
        std::cin >> choice;
    }

    return Barista::handleRequest(choice, drink);
}

Drink *Manager::handleRequest(char choice, Drink *drink)
{
    while (choice != 'd')
    {
        std::cout << "Manager prepares the coffee." << std::endl;

        switch (choice)
        {
        case 's':
            drink = new Sugar(drink);
            break;
        case 'c':
            drink = new Cream(drink);
            break;
        case 'f':
            drink = new MilkFoam(drink);
            break;
        }

        std::cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one?: ";
        std::cin >> choice;
    }

    return drink;
}

int main()
{
    char choice;
    Drink *drink;
    std::string name;

    JuniorBarista *steve = new JuniorBarista(new SeniorBarista(new Manager));
    drink = steve->handleRequest(choice, drink);

    std::cout << "Can I get your name?: ";
    std::cin >> name;

    std::cout << name << ", your " << drink->getName() << "is Ready. It will be $" << drink->getPrice() << ", please." << std::endl;
}