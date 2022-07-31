// jamming peaches
// non STL-algorithm code to be replaced by algorthms
// Seth Winkhart
// 2/26/2022

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>

using std::cin;
using std::cout;
using std::deque;
using std::endl;
using std::string;
using std::vector;

struct Peaches
{
    double weight; // oz
    bool ripe;     // ripe or not
    void print() const { cout << (ripe ? "ripe" : "green") << ", " << weight << endl; }
};

bool lessThan(Peaches p1, double maxWeight)
{
    return p1.weight < maxWeight;
}

int main()
{
    srand(time(nullptr));
    const double minWeight = 8.;
    const double maxWeight = 3.;

    cout << "Input basket size: ";
    int size;
    cin >> size;

    vector<Peaches> basket(size);

    // assign random weight and ripeness peaches in the basket
    // replace with generate()

    std::generate(basket.begin(), basket.end(), [=]()
                  {Peaches p;
                   p.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight);
                   p.ripe = rand() % 2;
                   return p; });

    // for_each() possibly
    cout << "all peaches" << endl;
    std::for_each(basket.begin(), basket.end(), [](Peaches p)
                  { p.print(); });

    // moving all the ripe peaches from basket to peck
    // remove_copy_if() with back_inserter()/front_inserter() or equivalents
    deque<Peaches> peck;

    std::remove_copy_if(basket.begin(), basket.end(), std::back_inserter(peck), [](Peaches p)
                        { return !(p.ripe); });

    // for_each() possibly
    cout << "peaches remainng in the basket" << endl;
    std::for_each(basket.begin(), basket.end(), [](Peaches p)
                  { p.print(); });

    cout << endl;

    // for_each() possibly
    cout << "peaches moved to the peck" << endl;
    std::for_each(peck.begin(), peck.end(), [](Peaches p)
                  { p.print(); });

    // prints every "space" peach in the peck
    //
    const int space = 3;
    cout << "\nevery " << space << "\'d peach in the peck" << endl;

    // replace with advance()/next()/distance()
    // no iterator arithmetic
    auto it = peck.cbegin();
    int i = 0;
    while (it != peck.cend())
    {
        if (i == space)
        {
            it->print();
            i = 0;
        }
        ++i;
        std::advance(it, 1);
    }

    // putting all small ripe peaches in a jam
    // use a binder to create a functor with configurable max weight
    // accumulate() or count_if() then remove_if()
    const double weightToJam = 10.0;
    double jamWeight = 0;

    using namespace std::placeholders;
    auto bndr = std::bind(lessThan, _1, weightToJam);
    int numPeaches = 0;
    auto itr = basket.begin();
    while (std::count_if(basket.begin(), basket.end(), bndr) != numPeaches)
    {
        if (itr->weight < weightToJam)
        {
            jamWeight += itr->weight;
            ++numPeaches;
        }
    }

    auto eraseItr = std::remove_if(basket.begin(), basket.end(), bndr);
    basket.erase(eraseItr, basket.end());
    cout << "Weight of jam is: " << jamWeight << endl;
}
