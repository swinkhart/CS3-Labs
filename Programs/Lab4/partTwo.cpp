// vector and list algorithms with objects in containers
// Seth Winkhart
// 2/15/2022

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
        cout << firstName_ + " " + lastName_ << ": ";
        for (auto course : classes_)
        {
            cout << course << " ";
        }
        cout << endl;
    }

    string getName() const { return firstName_ + " " + lastName_; }

    // function to add courses to student object, must be called after inital
    // construction becuase list is not initialized with constructor
    void addCourse(string course) { classes_.push_back(course); }

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
    list<string> classes_;
};

// read roster and compare it to main vector of lists
void readVectorRoster(vector<list<Student>> &master, list<Student> &roster, string fileName);
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

    // vector of courses of students
    vector<list<Student>> courseStudents;

    for (int i = 1; i < argc - 1; ++i)
    {
        list<Student> roster;
        readVectorRoster(courseStudents, roster, argv[i]);

        courseStudents.push_back(move(roster));
    }

    // reading in dropouts
    list<Student> dropouts;
    readRoster(dropouts, argv[argc - 1]);

    list<Student> allStudents; // master list of students

    for (auto &lst : courseStudents)
        allStudents.splice(allStudents.end(), lst);

    allStudents.sort(); // sorting master list

    allStudents.unique(); // eliminating duplicates

    cout << endl;
    cout << "All students, sorted" << endl;
    cout << "firstname, lastname: course" << endl;
    printRoster(allStudents);

    for (const auto &str : dropouts) // removing individual dropouts
        allStudents.remove(str);

    cout << endl;
    cout << "All students, sorted and dropouts removed" << endl;
    cout << "firstname, lastname: course" << endl;
    printRoster(allStudents);
}

// new read in function for the cs files, compares the read in names to the
// courseStudent vector of lists, presorting the roster list before it is pushed
// back to the courseStudents vector of lists. This preserves the master list
// allStudents and allows the splicing of courseStudents to allStudents as well
// as all subsequent functions performed on allStudents including removing
// dropouts.
void readVectorRoster(vector<list<Student>> &master, list<Student> &roster, string fileName)
{
    ifstream course(fileName);
    string first, last, courseName = fileName.substr(0, fileName.find('.'));
    while (course >> first >> last)
    {
        if (master.size() > 0)
        {
            for (int i = 0; i < master.size(); ++i)
            {
                for (auto &person : master[i])
                {
                    if ((first + " " + last) == person.getName())
                    {
                        person.addCourse(courseName);
                    }
                    else
                    {
                        Student tempStu(first, last);
                        tempStu.addCourse(courseName);
                        roster.push_back(tempStu);
                    }
                }
            }
        }
        else
        {
            Student tempStu(first, last);
            tempStu.addCourse(courseName);
            roster.push_back(tempStu);
        }
    }
}

void readRoster(list<Student> &roster, string fileName)
{
    ifstream course(fileName);
    string first, last, courseName = fileName.substr(0, fileName.find('.'));
    while (course >> first >> last)
        roster.push_back(Student(first, last));
    course.close();
}

// printing a list out
void printRoster(const list<Student> &roster)
{
    for (const auto &student : roster)
        student.print();
}