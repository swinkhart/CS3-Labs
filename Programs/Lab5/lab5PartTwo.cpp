// vector and list algorithms with objects in containers
// Seth Winkhart
// 9/10/2018

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::list;
using std::make_pair;
using std::map;
using std::move;
using std::set;
using std::string;
using std::vector;

class Student
{
public:
    Student(string firstName, string lastName) : firstName_(firstName), lastName_(lastName) {}

    // move constructor, not really needed, generated automatically
    Student(Student &&org) : firstName_(move(org.firstName_)),
                             lastName_(move(org.lastName_))
    {
    }

    // force generation of default copy constructor
    Student(const Student &org) = default;

    void print() const
    {
        cout << firstName_ + ' ' + lastName_;
    }

    string getName() const { return firstName_ + " " + lastName_; }

    // needed for unique() and for remove()
    friend bool operator==(Student left, Student right)
    {
        return left.lastName_ == right.lastName_ &&
               left.firstName_ == right.firstName_;
    }

    // needed for sort()
    friend bool operator<(Student left, Student right)
    {
        return left.lastName_ < right.lastName_ ||
               (left.lastName_ == right.lastName_ &&
                left.firstName_ < right.firstName_);
    }

private:
    string firstName_;
    string lastName_;
};

// reading a list from a fileName
void readRoster(list<Student> &roster, string fileName);
// printing a list out
void printRoster(const list<Student> &roster);

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        cout << "usage: " << argv[0]
             << " list of courses, dropouts last" << endl;
        exit(1);
    }

    // map of students and the courses they are taking
    set<Student> allStudents;

    for (int i = 1; i < argc - 1; ++i)
    {
        string fileName = argv[i];
        ifstream course(fileName);
        string first, last;
        while (course >> first >> last)
        {
            if (allStudents.size() > 0)
            {
                for (auto person : allStudents)
                {
                    if ((first + ' ' + last) != person.getName())
                    {
                        Student tempStudent(first, last);
                        allStudents.emplace(tempStudent);
                    }
                }
            }
            else
            {
                Student tempStudent(first, last);
                allStudents.emplace(tempStudent);
            }
        }
        course.close();
    }

    // reading in dropouts
    set<Student> dropouts;
    ifstream drops(argv[argc - 1]);
    string first, last;
    while (drops >> first >> last)
    {
        Student tempStudent(first, last);
        dropouts.emplace(tempStudent);
    }
    drops.close();

    // prints all students in atleast one course, basically all students minus
    // the dropouts
    bool failure = false;
    cout << endl;
    cout << "Currently enrolled students:" << endl;
    for (auto person : allStudents)
    {
        for (auto dropout : dropouts)
        {
            if (person.getName() == dropout.getName())
            {
                failure = true;
            }
        }

        if (failure == false)
        {
            person.print();
            cout << endl;
        }

        // reset bool for next iteration
        failure = false;
    }
}