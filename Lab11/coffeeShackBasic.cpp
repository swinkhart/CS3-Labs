// coffee shack decorator
// Seth Winkhart
// 4/12/2022

#include <string>
#include <iostream>

enum class DrinkType
{
    small,
    medium,
    large
};

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

// member functions
std::string Drink::getName() const
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

int main()
{
    char size;
    char add;
    DrinkType type;
    int price;
    std::string name;
    Drink *myDrink;

    std::cout << "Welcome to Ripoff Shack, can I get you a [l]arge, [m]edium, or [s]mall coffee?: ";
    std::cin >> size;

    switch (size)
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

    myDrink = new Drink(type, price);

    do
    {
        std::cout << "would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one?: ";
        std::cin >> add;

        switch (add)
        {
        case 's':
            myDrink = new Sugar(myDrink);
            break;
        case 'c':
            myDrink = new Cream(myDrink);
            break;
        case 'f':
            myDrink = new MilkFoam(myDrink);
            break;
        }

    } while (add != 'd');

    std::cout << "Can I get your name?: ";
    std::cin >> name;

    std::cout << name << ", your " << myDrink->getName() << "is Ready. It will be $" << myDrink->getPrice() << ", please." << std::endl;
}