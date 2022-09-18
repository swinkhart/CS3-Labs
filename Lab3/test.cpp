// testing the implementation of templated list collection
// Mikhail Nesterenko
// 9/6/2021

#include <iostream>
#include <string>
#include "Collection.hpp" // template definition

using std::cout;
using std::endl;
using std::string;

int main()
{

   // manipulating integers
   Collection<int> cone;
   cout << "Integer collection: ";
   cout << "adding 1 ";
   cone.add(1);
   cout << "adding 2 ";
   cone.add(2);
   cout << "adding 3 ";
   cone.add(3);
   cout << "adding duplicate 2 ";
   cone.add(2);
   cout << endl;

   cone.print();

   cout << "removing 2 ";
   cone.remove(2);
   cout << endl;
   ;

   cone.print();

   if (equal(cone, cone))
      cout << "cone list is equal to itself" << endl;

   // uncomment when you debugged the code above

   // manipulating strings
   string sa[] = {"yellow", "orange", "green", "blue"};
   Collection<string> ctwo;

   cout << "adding an array of strings ";
   for (auto s : sa)
      ctwo.add(s);

   cout << "String collection: ";
   ctwo.print();

   // manipulating character collections

   // individal collections
   Collection<char> a2g, h2n, o2u;
   cout << "Populating individual character collections" << endl;
   for (char c = 'a'; c <= 'g'; ++c)
      a2g.add(c);
   for (char c = 'h'; c <= 'n'; ++c)
      h2n.add(c);
   for (char c = 'o'; c <= 'u'; ++c)
      o2u.add(c);

   if (!equal(a2g, h2n))
      cout << "a2g is not equal to h2n" << endl;

   // collection of collections
   Collection<Collection<char>> cpile;

   cout << "Adding individual collections " << endl;
   cpile.add(a2g);
   cpile.add(h2n);
   cpile.add(o2u);

   // printing characters from last collection added
   cout << "Last added character collection: ";
   cpile.last().print();
}