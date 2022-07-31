// sorting apples
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

using std::cin;
using std::cout;
using std::deque;
using std::endl;
using std::string;
using std::vector;

struct Apples
{
    double weight; // oz
    string color;  // red or green
    void print() const { cout << color << ", " << weight << endl; }
};

int main()
{
    srand(time(nullptr));
    const double minWeight = 8.;
    const double maxWeight = 3.;

    cout << "Input crate size: ";
    int size;
    cin >> size;

    vector<Apples> crate(size);

    // assign random weight and color to apples in the crate
    // replace with generate()
    std::generate(crate.begin(), crate.end(), [=]()
                  {Apples app;
                   app.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight);
                   app.color = rand() % 2 == 1 ? "green" : "red";
                   return app; });

    // for_each() possibly
    std::for_each(crate.begin(), crate.end(), [](Apples app)
                  { app.print(); });

    cout << "Enter weight to find: ";
    double toFind;
    cin >> toFind;

    // count_if()
    int cnt = std::count_if(crate.begin(), crate.end(), [toFind](Apples app)
                            { return app.weight > toFind; });

    cout << "There are " << cnt << " apples heavier than " << toFind << " oz" << endl;

    // find_if()
    cout << "at positions ";
    auto itr = std::find_if(crate.begin(), crate.end(), [toFind](Apples app)
                            { return app.weight > toFind; });
    while (itr != crate.end())
    {
        cout << itr - crate.begin() << ", ";
        itr = std::find_if((itr + 1), crate.end(), [toFind](Apples app)
                           { return app.weight > toFind; });
    }
    cout << endl;

    // max_element()
    itr = std::max_element(crate.begin(), crate.end(), [](Apples appA, Apples appB)
                           { return appA.weight < appB.weight; });
    cout << "Heaviest apple weighs: " << itr->weight << " oz" << endl;

    // for_each() or accumulate()
    auto sum = std::accumulate(crate.begin(), crate.end(), 0.0, [](auto total, Apples app)
                               { return total += app.weight; });
    cout << "Total apple weight is: " << sum << " oz" << endl;

    // transform();
    cout << "How much should they grow: ";

    double toGrow;
    cin >> toGrow;
    std::transform(crate.begin(), crate.end(), crate.begin(), [toGrow](Apples app)
                   { Apples tmp;
                   tmp.color = app.color;
                   tmp.weight = app.weight + toGrow;
                   return tmp; });

    // remove_if()
    cout << "Input minimum acceptable weight: ";
    double minAccept;
    cin >> minAccept;

    crate.erase(std::remove_if(crate.begin(), crate.end(), [minAccept](Apples &app)
                               { return app.weight < minAccept; }),
                crate.end());

    cout << "removed " << size - crate.size() << " elements" << endl;

    // bubble sort, replace with sort()
    std::sort(crate.begin(), crate.end(), [](Apples appA, Apples appB)
              { return appA.weight < appB.weight; });

    // for_each() possibly
    cout << "sorted remaining apples" << endl;
    std::for_each(crate.begin(), crate.end(), [](Apples app)
                  { app.print(); });
}