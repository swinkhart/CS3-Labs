// coffee shack decorator, chain of responsibility, and observer
// Seth Winkhart
// 4/18/2022

#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

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

// chain of responsibility and observer pattern stuff
// administrator/concrete subject
class Barista
{
public:
    // chain of responsiblity functions
    Barista(Barista *successor = nullptr) : successor_(successor) {}
    virtual Drink *handleRequest(char choice, std::queue<Drink *> &drink) { return successor_->handleRequest(choice, drink); }

    // observer functions
    void registerObserver(class Customer *c) { orders_.push_back(c); }
    void deregisterObserver(int index) { orders_.erase(orders_.begin() + index); }
    void notifyObserver() const;
    void completeOrder();
    bool everyCustomerServed() { return orders_.empty(); }

    std::string getCustomerName() const;

protected:
    static std::vector<class Customer *> orders_; // orders shared between all instances of Barista
    int customerNumber_;

private:
    Barista *successor_;
};

// concrete observer
class Customer
{
public:
    Customer(const std::string &name, class Barista *subject, Drink *drink) : name_(name), subject_(subject), drink_(drink) { subject_->registerObserver(this); }

    void notify() const;
    std::string getName() const { return name_; }
    Drink *getDrink() const { return drink_; }

private:
    std::string name_;
    Drink *drink_;
    class Barista *subject_;
};

std::vector<Customer *> Barista::orders_; // initialize static vector

// derived chain on responsibility classes
class JuniorBarista : public Barista
{
public:
    JuniorBarista(Barista *successor = nullptr) : Barista(successor) {}
    Drink *handleRequest(char choice, std::queue<Drink *> &drink) override;
};

class SeniorBarista : public Barista
{
public:
    SeniorBarista(Barista *successor = nullptr) : Barista(successor) {}
    Drink *handleRequest(char choice, std::queue<Drink *> &drink) override;
};

class Manager : public Barista
{
public:
    Manager(Barista *successor = nullptr) : Barista(successor) {}
    Drink *handleRequest(char choice, std::queue<Drink *> &drink) override;
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

Drink *JuniorBarista::handleRequest(char choice, std::queue<Drink *> &drink)
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

    drink.push(new Drink(type, price));

    std::cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one?: ";
    std::cin >> choice;

    if (choice == 'd')
    {
        std::cout << "Junior barista prepares the coffee." << std::endl;
        return drink.back();
    }
    else
    {
        return Barista::handleRequest(choice, drink);
    }
}

Drink *SeniorBarista::handleRequest(char choice, std::queue<Drink *> &drink)
{
    while (choice != 'f')
    {
        std::cout << "Senior Barista prepares the coffee." << std::endl;

        switch (choice)
        {
        case 's':
            drink.push(new Sugar(drink.back()));
            break;
        case 'c':
            drink.push(new Cream(drink.back()));
            break;
        case 'd':
            return drink.back();
            break;
        }

        std::cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one?: ";
        std::cin >> choice;
    }

    return Barista::handleRequest(choice, drink);
}

Drink *Manager::handleRequest(char choice, std::queue<Drink *> &drink)
{
    while (choice != 'd')
    {
        std::cout << "Manager prepares the coffee." << std::endl;

        switch (choice)
        {
        case 's':
            drink.push(new Sugar(drink.back()));
            break;
        case 'c':
            drink.push(new Cream(drink.back()));
            break;
        case 'f':
            drink.push(new MilkFoam(drink.back()));
            break;
        }

        std::cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one?: ";
        std::cin >> choice;
    }

    return drink.back();
}

std::string Barista::getCustomerName() const
{
    return orders_[customerNumber_]->getName();
}

void Customer::notify() const
{
    std::cout << "Customer " << name_ << " the order for" << subject_->getCustomerName() << " is finished" << std::endl;
}

void Barista::notifyObserver() const
{
    for (auto customer : orders_)
    {
        customer->notify();
    }
}

void Barista::completeOrder()
{
    customerNumber_ = rand() % orders_.size();

    this->notifyObserver();

    std::cout << orders_[customerNumber_]->getName() << " your " << orders_[customerNumber_]->getDrink()->getName() << " is ready, it will be $" << orders_[customerNumber_]->getDrink()->getPrice() << " please" << std::endl;

    this->deregisterObserver(customerNumber_);
}

int main()
{
    const int reqNumCustomers = 2;

    srand(time(nullptr));

    JuniorBarista *steve = new JuniorBarista(new SeniorBarista(new Manager));
    int currServedCustomer = 0;
    do
    {
        int choice = rand() % 2;
        if (choice == 0 && currServedCustomer != reqNumCustomers)
        {
            char size;
            std::string name;
            std::queue<Drink *> drink;
            steve->handleRequest(size, drink);

            std::cout << "Can I get your name?: ";
            std::cin >> name;

            Customer *newCustomer = new Customer(name, steve, drink.back());
            ++currServedCustomer;
        }
        else if (choice == 1 && !(steve->everyCustomerServed()))
        {
            steve->completeOrder(); // finish order
        }
    } while (currServedCustomer < reqNumCustomers || !(steve->everyCustomerServed()));
}