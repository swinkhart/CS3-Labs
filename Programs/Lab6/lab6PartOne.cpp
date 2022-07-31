// selecting oranges
// converting vectors to multimaps
// Seth Winkhart
// 2/22/2022

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::move;
using std::multimap;
using std::string;
using std::vector;

enum class Variety
{
    orange,
    pear,
    apple
};
vector<string> colors = {"red", "green", "yellow"};

int main()
{
    srand(time(nullptr));
    multimap<Variety, string> Fruit;

    int numFruit = rand() % 100 + 1;
    cout << "number of fruit: " << numFruit << endl;
    for (int i = 0; i <= numFruit; ++i)
    {
        Variety tempVar = static_cast<Variety>(rand() % 3);
        string tempColor = colors[rand() % 3];
        Fruit.emplace(move(make_pair(tempVar, tempColor)));
    }

    cout << "Colors of the oranges: ";
    for (auto f = Fruit.lower_bound(Variety::orange); f != Fruit.upper_bound(Variety::orange); ++f)
    {
        cout << f->second << ", ";
    }
}