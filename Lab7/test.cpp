// hashmap test
// Seth Winkhart
// 03/15/2022

#include "hashmap.hpp"
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
    // create hashmap
    hashmap<int, int> myHashMap;

    // testing insert, first two should work the third should not
    auto result1 = myHashMap.insert(make_pair(1, 11));

    if (result1.second)
    {
        cout << "success 1" << endl;
    }
    else
    {
        cout << "failed 1" << endl;
    }

    auto result2 = myHashMap.insert(make_pair(2, 22));

    if (result2.second)
    {
        cout << "success 2" << endl;
    }
    else
    {
        cout << "failed 2" << endl;
    }

    auto result3 = myHashMap.insert(make_pair(2, 33));

    if (result3.second)
    {
        cout << "success 3" << endl;
    }
    else
    {
        cout << "failed 3" << endl;
    }

    // tesing index operator

    // unsafe insert even though is uses insert in the function definition
    // making it safe;
    myHashMap[3] = 33;

    // retrieving element usi"ng indexing operator
    cout << myHashMap[3] << ": this should be 33" << endl;

    // tesing erase
    auto result = myHashMap.erase(3);

    if (result.second)
    {
        cout << "success" << endl;
    }

    // testing rehash
    myHashMap.rehash(201);
    cout << "the new number of buckets is: " << myHashMap.getNumBuckets() << " and not 101" << endl;

    // checking if things are still correctly hashed in the hashmap
    cout << myHashMap[1] << " :this should be 11" << endl;
    cout << myHashMap[2] << " :this should be 33" << endl;
}
//*/