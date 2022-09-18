// modified car lot class
// Seth Winkhart
// 3/28/2022

#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>
#include "CarFactory.hpp"

using std::cout;
using std::endl;
using std::vector;

class CarLot
{
public:
    CarLot()
    {
        // creates 2 Ford factories and 2 Toyota factories
        factories_.push_back(new FordFactory());
        factories_.push_back(new ToyotaFactory());
        factories_.push_back(new FordFactory());
        factories_.push_back(new ToyotaFactory());

        // fills lot with 10 random cars
        for (int i = 0; i < lotSize_; ++i)
        {
            car4sale_[i] = *(factories_[rand() % factories_.size()]->requestCar());
        }
    }

    // if a car is bought, requests a new one
    Car *buyCar()
    {
        Car *bought = &car4sale_[currCar_];
        car4sale_[currCar_] = *(factories_[rand() % factories_.size()]->requestCar());
        return bought;
    }

    Car *nextCar()
    {
        ++currCar_;
        if (currCar_ == lotSize_)
        {
            currCar_ = 0;
        }
        Car *result = &car4sale_[currCar_];
        return result;
    }

    int lotSize() { return lotSize_; }

private:
    static const int lotSize_ = 10;
    Car car4sale_[lotSize_]; // lot of cars for sale
    vector<CarFactory *> factories_;
    static int currCar_;
};

int CarLot::currCar_ = 0;

CarLot *carLotPtr = nullptr; // global pointer instantiation

// test-drives a car
// buys it if Toyota
void toyotaLover(int id)
{
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    static const std::array<std::string, 5> toyotaModels = {"Corolla", "Camry", "Prius", "4Runner", "Yaris"};

    std::string ModelToBuy = toyotaModels[rand() % toyotaModels.size()];

    for (int i = 0; i < carLotPtr->lotSize(); ++i)
    {
        Car *toBuy = carLotPtr->nextCar();
        cout << "Buyer " << id << " test driving "
             << toBuy->getMake() << " "
             << toBuy->getModel() << ", they";

        if (toBuy->getMake() == "Toyota" && toBuy->getModel() == ModelToBuy)
        {
            cout << " love it! buying it!" << endl;
            carLotPtr->buyCar();
            break;
        }
        else
            cout << " did not like it!" << endl;
    }
}

// test-drives a car
// buys it if Ford
void fordLover(int id)
{
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    static const std::array<std::string, 4> fordModels = {"Focus", "Mustang", "Explorer", "F-150"};

    std::string ModelToBuy = fordModels[rand() % fordModels.size()];

    for (int i = 0; i < carLotPtr->lotSize(); ++i)
    {
        Car *toBuy = carLotPtr->nextCar();
        cout << "Buyer " << id << " test driving "
             << toBuy->getMake() << " "
             << toBuy->getModel() << ", they";

        if (toBuy->getMake() == "Ford" && toBuy->getModel() == ModelToBuy)
        {
            cout << " love it! buying it!" << endl;
            carLotPtr->buyCar();
            break;
        }
        else
            cout << " did not like it!" << endl;
    }
}

int main()
{
    srand(time(nullptr));

    const int numBuyers = 20;
    for (int i = 0; i < numBuyers; ++i)
        if (rand() % 2 == 0)
            toyotaLover(i);
        else
            fordLover(i);
}
