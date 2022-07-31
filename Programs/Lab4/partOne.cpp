// vector and list algorithms
// Seth Winkhart
// 2/14/2022

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::list;
using std::move;
using std::string;
using std::vector;

// reads a vector of lists rather than a list
void readVectorRoster(vector<list<string>> &roster, string fileName);

// reading a list from a fileName
void readRoster(list<string> &roster, string fileName);

// printing a list out
void printRoster(const list<string> &roster);

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        cout << "usage: " << argv[0]
             << " list of courses, dropouts last"
             << endl;
        exit(1);
    }

    // temporary vector of courses of students
    vector<list<string>> tempCourseStudents;

    for (int i = 1; i < argc - 1; ++i)
    {
        list<string> roster;
        readRoster(roster, argv[i]);

        cout << "\n\n"
             << argv[0] << "\n";
        printRoster(roster);

        tempCourseStudents.push_back(move(roster));
    }

    // reading in dropouts
    list<string> dropouts;
    readRoster(dropouts, argv[argc - 1]);
    // cout << "\n\n dropouts \n";
    // printRoster(dropouts);

    // master list of students
    list<string> allStudents;

    for (auto &lst : tempCourseStudents)
        allStudents.splice(allStudents.end(), lst);

    // cout << "\n\n all students unsorted \n";
    // printRoster(allStudents);

    // recreate course students since the origianl was destroyed during splicing
    vector<list<string>> courseStudents;

    for (int i = 1; i < argc - 1; ++i)
    {
        list<string> roster;
        readRoster(roster, argv[i]);

        cout << "\n\n"
             << argv[0] << "\n";
        printRoster(roster);

        courseStudents.push_back(move(roster));
    }

    // sorting master list
    allStudents.sort();
    // cout << "\n\n all students sorted \n";
    // printRoster(allStudents);

    // eliminating duplicates
    allStudents.unique();
    // cout << "\n\n all students, duplicates removed \n";
    // printRoster(allStudents);

    cout << endl;
    cout << endl;
    cout << "All students sorted" << endl;
    cout << "first name, last name: course" << endl;
    for (auto name : allStudents)
    {
        cout << name << ": ";
        for (int i = 0; i < courseStudents.size(); ++i)
        {
            for (auto lst : courseStudents[i])
            {
                if (name == lst)
                {
                    cout << "cs" << i + 1 << " ";
                }
            }
        }
        cout << endl;
    }

    // removing individual dropouts
    for (const auto &str : dropouts)
        allStudents.remove(str);

    // cout << "\n\n all students, dropouts removed \n";
    // printRoster(allStudents);

    cout << endl;
    cout << endl;
    cout << "All students sorted and dropout removed" << endl;
    cout << "first name, last name: course" << endl;
    for (auto name : allStudents)
    {
        cout << name << ": ";
        for (int i = 0; i < courseStudents.size(); ++i)
        {
            for (auto lst : courseStudents[i])
            {
                if (name == lst)
                {
                    cout << "cs" << i + 1 << " ";
                }
            }
        }
        cout << endl;
    }
}

// reading in a file of names into a list of strings
void readRoster(list<string> &roster, string fileName)
{
    ifstream course(fileName);
    string first, last;
    while (course >> first >> last)
        roster.push_back(move(first + ' ' + last));
    course.close();
}

// printing a list out
void printRoster(const list<string> &roster)
{
    for (const auto &str : roster)
        cout << str << endl;
}
