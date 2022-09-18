// traversing modified genealogical tree using Composite and Visitors
// Seth Winkhart
// 4/21/2022

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class Person
{ // component
public:
    Person(string firstName, Person *spouse, Person *father, Person *mother) : firstName_(firstName), spouse_(spouse), father_(father), mother_(mother) {}
    const string &getFirstName() { return firstName_; }
    Person *getSpouse() { return spouse_; }
    void setSpouse(Person *spouse) { spouse_ = spouse; }
    Person *getFather() { return father_; }
    Person *getMother() { return mother_; }

    virtual void accept(class PersonVisitor *) = 0;
    virtual const string getName() = 0;
    virtual const vector<Person *> &getChildren() = 0;
    virtual ~Person() {}

private:
    const string firstName_;
    Person *spouse_;
    Person *father_;
    Person *mother_;
};

// leaf
// man has a last name
class Man : public Person
{
public:
    Man(string lastName, string firstName, Person *spouse,
        Person *father, Person *mother) : lastName_(lastName),
                                          Person(firstName, spouse, father, mother) {}
    const string getLastName() { return lastName_; }
    void accept(class PersonVisitor *visitor) override;
    const vector<Person *> &getChildren() { return getSpouse()->getChildren(); }
    const string getName() override { return getFirstName() + " " + getLastName(); }

private:
    const string lastName_;
};

// composite
// woman has a list of children
class Woman : public Person
{
public:
    Woman(vector<Person *> children,
          string firstName,
          Person *spouse,
          Person *father, Person *mother) : children_(children),
                                            Person(firstName, spouse, father, mother) {}
    const vector<Person *> &getChildren() { return children_; }
    void setChildren(const vector<Person *> &children) { children_ = children; }
    void accept(class PersonVisitor *visitor) override;
    const string getName() override;

private:
    vector<Person *> children_;
};

// abstract visitor
class PersonVisitor
{
public:
    virtual void visit(Man *) = 0;
    virtual void visit(Woman *) = 0;
    virtual ~PersonVisitor() {}
};

// composite methods that define traversal
void Man::accept(PersonVisitor *visitor)
{
    visitor->visit(this);
}

void Woman::accept(PersonVisitor *visitor)
{
    // children traversal through mother only
    // father's children are not traversed so as not
    // to traverse them twice: for mother and father

    visitor->visit(this);

    // traversing descendants
    for (auto child : children_)
        child->accept(visitor);
}

const string Woman::getName()
{
    string fullName;
    fullName = getFirstName();
    if (getSpouse() != nullptr)
        return fullName = fullName + " " + static_cast<Man *>(getSpouse())->getLastName();
    else if (getFather() != nullptr)
        return fullName = fullName + " " + static_cast<Man *>(getFather())->getLastName();
    else
        return fullName;
}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her
// spouse if she is married
// or by her father if she is not
class NamePrinter : public PersonVisitor
{
public:
    void visit(Man *m) override
    {
        cout << m->getFirstName() << " " << m->getLastName() << endl;
    }
    void visit(Woman *w) override
    {
        cout << w->getFirstName() << " ";
        if (w->getSpouse() != nullptr)
            cout << static_cast<Man *>(w->getSpouse())->getLastName();
        else if (w->getFather() != nullptr)
            cout << static_cast<Man *>(w->getFather())->getLastName();
        else
            cout << "Doe";
        cout << endl;
    }
};

class ChildrenPrinter : public PersonVisitor
{
public:
    void visit(Man *m) override
    {
        cout << m->getFirstName() << ": ";
        Woman *spouse = static_cast<Woman *>(m->getSpouse());
        if (spouse != nullptr)
            printNames(spouse->getChildren());
        cout << endl;
    }
    void visit(Woman *w) override
    {
        cout << w->getFirstName() << ": ";
        printNames(w->getChildren());
        cout << endl;
    }

private:
    void printNames(const vector<Person *> &children)
    {
        for (const auto c : children)
            cout << c->getFirstName() << ", ";
    }
};

class RelationalCheck : public PersonVisitor
{
public:
    RelationalCheck(string firstPerson, string secondPerson) : firstPerson_(firstPerson), secondPerson_(secondPerson) {}
    bool getJustFriends() const { return justFriends_; }

    void visit(Man *m) override
    {
        if (m->getName() == firstPerson_) // check if current node is person 1
        {
            if (m->getName() == secondPerson_) // self check
            {
                justFriends_ = false;
                return;
            }
            else if (m->getFather() != nullptr && m->getFather()->getName() == secondPerson_) // father check
            {
                justFriends_ = false;
                return;
            }
            else if (m->getMother() != nullptr && m->getMother()->getName() == secondPerson_) // mother check
            {
                justFriends_ = false;
                return;
            }
            else if (m->getSpouse() != nullptr) // children check
            {
                for (auto child : m->getSpouse()->getChildren())
                {
                    if (child->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
            else if (m->getMother() != nullptr) // siblings check
            {
                for (auto sibling : m->getMother()->getChildren())
                {
                    if (sibling->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
            else if (m->getMother()->getMother() != nullptr) // aunts and uncles on mothers side check
            {
                for (auto sibling : m->getMother()->getMother()->getChildren())
                {
                    if (sibling->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
            else if (m->getFather()->getMother() != nullptr) // aunts and uncles on fathers side check
            {
                for (auto sibling : m->getFather()->getMother()->getChildren())
                {
                    if (sibling->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
        }
    }

    void visit(Woman *w) override
    {
        if (w->getName() == firstPerson_) // check if current node is person 1
        {
            if (w->getName() == secondPerson_) // check self
            {
                justFriends_ = false;
                return;
            }
            else if (w->getFather() != nullptr && w->getFather()->getName() == secondPerson_) // check father
            {
                justFriends_ = false;
                return;
            }
            else if (w->getMother() != nullptr && w->getMother()->getName() == secondPerson_) // check mother
            {
                justFriends_ = false;
                return;
            }
            else if (w->getSpouse() != nullptr) // check children
            {
                for (auto child : w->getChildren())
                {
                    if (child->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
            else if (w->getMother() != nullptr) // check siblings
            {
                for (auto sibling : w->getMother()->getChildren())
                {
                    if (sibling->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
            else if (w->getMother()->getMother() != nullptr) // check aunts and uncles on mothers side
            {
                for (auto child : w->getMother()->getMother()->getChildren())
                {
                    if (child->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
            else if (w->getFather()->getMother() != nullptr) // check aunts and uncles on fathers side
            {
                for (auto child : w->getFather()->getMother()->getChildren())
                {
                    if (child->getName() == secondPerson_)
                    {
                        justFriends_ = false;
                        return;
                    }
                }
            }
        }
    }

private:
    bool justFriends_ = true;
    string firstPerson_;
    string secondPerson_;
};

// demonstrating the operation
int main()
{

    // setting up the genealogical tree
    // the tree is as follows
    //
    //
    //       James Smith  <--spouse-->   Mary
    //	                                  |
    //	                                 children -------------------------
    //	                                  |              |                |
    //	                                  |              |                |
    //	   William Johnson <--spouse-> Patricia      Robert Smith       Linda
    //	                                  |
    //	                                 children------------
    //	                                  |                 |
    //                                     |                 |
    //	   Jennifer  <--spouse-->  Michael Johnson      Barbara
    //	       |
    //	     children
    //	       |
    //          |
    //	     Susan

    // first generation
    Man *js = new Man("Smith", "James", nullptr, nullptr, nullptr);
    Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
    ms->setSpouse(js);
    js->setSpouse(ms);

    // second generation
    Woman *ps = new Woman({}, "Patricia", nullptr, js, ms);
    Man *wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
    ps->setSpouse(wj);
    wj->setSpouse(ps);

    vector<Person *> marysKids = {ps,
                                  new Man("Smith", "Robert", nullptr, js, ms),
                                  new Woman({}, "Linda", nullptr, js, ms)};
    ms->setChildren(marysKids);

    // third generation
    Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps);
    vector<Person *> patsKids = {mj, new Woman({}, "Barbara", nullptr, wj, ps)};
    ps->setChildren(patsKids);

    Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
    vector<Person *> jensKids = {new Woman({}, "Susan", nullptr, mj, jj)};

    jj->setSpouse(mj);
    mj->setSpouse(jj);
    jj->setChildren(jensKids);

    // defining two visitors
    ChildrenPrinter *cp = new ChildrenPrinter;
    NamePrinter *np = new NamePrinter;

    // executing the traversal with the composite
    // and the specific visitor

    // cout << "\nNAME LIST\n";
    // ms->accept(np);
    // cout << endl << endl;

    // cout << "CHILDREN LIST\n";
    // ms->accept(cp);

    // cout << "\nJAMES' CHILDREN\n";
    // js->accept(cp);

    string person1;
    string person2;

    cout << "Enter first candidate: ";
    std::getline(std::cin, person1);
    cout << "Enter second candidate: ";
    std::getline(std::cin, person2);

    RelationalCheck *test = new RelationalCheck(person1, person2);
    ms->accept(test);

    if (test->getJustFriends())
    {
        cout << "They're just friends";
    }
    else
    {
        cout << "They're close relatives";
    }
}