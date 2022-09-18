// hashmap test file
// Mikhail Nesterenko,  Tsung-Heng Wu
// 10/18/2021

#include "hashmap.hpp"
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

// template tests hashmap inserts
template <typename Key, typename Value>
void test_insert(hashmap<Key, Value> &hm, Key key, Value value)
{
    cout << "[*] Inserting " << key << ": " << value << "...\n";
    pair<const pair<const Key, Value> *, bool> result = hm.insert(make_pair(key, value));

    if (result.second)
        cout << "--> Done!\n";
    else
        cout << "--> Failed! Key " << result.first->first
             << " already exists with value " << result.first->second << endl;
}

// template tests hashmap erases
template <typename Key, typename Value>
void test_erase(hashmap<Key, Value> &hm, Key key)
{
    cout << "[*] Erasing " << key << "...\n";
    pair<const pair<const Key, Value> *, bool> result = hm.erase(key);

    if (result.second)
    {
        cout << "--> Done! ";
        if (result.first)
            cout << "The next element is " << result.first->first
                 << ": " << result.first->second << endl;
        else
            cout << "There is no next element\n";
    }
    else
    {
        cout << "--> Failed! Key " << key << " doesn't exist!\n";
    }
}

int main()
{

    //
    // <int, int> hashmap test
    //
    cout << endl
         << "testing <int, int> hashmap" << endl;
    hashmap<int, int> myHash;

    // test inserts
    test_insert(myHash, 4, 40);
    test_insert(myHash, 6, 60);
    test_insert(myHash, 6, 70); // insert with a duplicate key

    // searching map
    // x will have type hashmap<int, int>::value_type*
    auto x = myHash.find(4);
    if (x != nullptr)
        cout << "4 maps to " << x->second << endl;
    else
        cout << "cannot find 4 in map" << endl;

    // test deletes
    test_erase(myHash, 4);
    test_erase(myHash, 6);
    test_erase(myHash, 7); // erase non-existent key

    //
    // <integer, string> hashmap test
    //
    cout << endl
         << "testing <int, string> hashmap" << endl;
    hashmap<int, string> employees;

    employees[123] = "Mike";
    employees[345] = "Charlie";
    employees[192] = "Joe";
    employees[752] = "Paul";
    employees[328] = "Peter";

    cout << "Enter new employee number: ";
    int num;
    cin >> num;
    cout << "Enter new employee name: ";
    string name;
    cin >> name;
    test_insert<int, string>(employees, num, name);

    // searching map
    cout << "Enter employee number to look for: ";
    cin >> num;
    auto it = employees.find(num);
    if (it != nullptr)
        cout << it->first << ":" << it->second << endl;
    else
        cout << "employee not found" << endl;

    // removing from a map
    cout << "Enter employee number to fire: ";
    cin >> num;
    test_erase(employees, num);

    //
    // testing rehash
    //
    cout << endl
         << "testing rehash" << endl;
    employees.rehash(201);
    cout << "[*] New number of buckets is " << employees.getNumBuckets() << endl;

    // testing if hashmap is still correct after rehash
    cout << 123 << ": " << employees[123] << endl;
    cout << 752 << ": " << employees[752] << endl;

    // testing insert
    test_insert<int, string>(employees, 999, "Susan");
}
