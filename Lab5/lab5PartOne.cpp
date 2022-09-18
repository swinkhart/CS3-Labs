// map containers with student keys
// Seth Winkhart
// 2/19/2022

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <map>

using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::list;
using std::make_pair;
using std::map;
using std::move;
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
        cout << firstName_ + ' ' + lastName_ + ": ";
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
    map<Student, list<string>> courseStudents;

    for (int i = 1; i < argc - 1; ++i)
    {
        string fileName = argv[i];
        ifstream course(fileName);
        string first, last, courseName = fileName.substr(0, fileName.find('.'));
        while (course >> first >> last)
            if (!courseStudents.empty())
            {
                for (auto &person : courseStudents)
                {
                    if (person.first.getName() == first + ' ' + last)
                    {
                        person.second.push_back(courseName);
                    }
                    else
                    {
                        Student tempStudent(first, last);
                        list<string> tempList({courseName});
                        courseStudents.insert(make_pair(tempStudent, tempList));
                    }
                    person.second.unique();
                }
            }
            else
            {
                Student tempStudent(first, last);
                list<string> tempList(1, courseName);
                courseStudents.insert(make_pair(tempStudent, tempList));
            }
        course.close();
    }

    ///*
    // reading in dropouts
    list<Student> dropouts;
    ifstream drops(argv[argc - 1]);
    string first, last;
    while (drops >> first >> last)
        dropouts.push_back(Student(first, last));
    drops.close();

    // print all students and the classes they are taking
    cout << "All students, sorted" << endl;
    cout << "firstname, lastname: course" << endl;
    for (auto person : courseStudents)
    {
        person.first.print();
        for (auto course : person.second)
        {
            cout << course << ' ';
        }
        cout << endl;
    }

    // print all students and classes the are taking, dropouts removed
    cout << endl;
    cout << "All students, sorted and dropouts removed" << endl;
    cout << "firstname, lastname: course" << endl;

    bool failure = false;
    for (auto person : courseStudents)
    {
        for (auto dropout : dropouts)
        {

            if (person.first.getName() == dropout.getName())
            {
                failure = true;
            }
        }

        if (failure == false)
        {
            person.first.print();
            for (auto course : person.second)
            {
                cout << course << ' ';
            }
            cout << endl;
        }

        // reset bool for next iteration
        failure = false;
    }
}