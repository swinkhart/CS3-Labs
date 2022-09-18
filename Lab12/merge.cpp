// Merge command and memento
// Seth Winkhart
// 4/22/2022

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// receiver
class Document
{
public:
    Document(const vector<string> &lines = {}) : lines_(lines) {}

    // actions
    void insert(int line, const string &str)
    {
        const int index = line - 1;
        if (index <= lines_.size())
            lines_.insert(lines_.begin() + index, str);
        else
            cout << "line out of range" << endl;
    }

    string remove(int line)
    {
        const int index = line - 1;
        string deletedLine = "";
        if (index < lines_.size())
        {
            deletedLine = lines_[index];
            lines_.erase(lines_.begin() + index);
        }
        else
            cout << "line out of range" << endl;
        return deletedLine;
    }

    void show()
    {
        for (int i = 0; i < lines_.size(); ++i)
            cout << i + 1 << ". " << lines_[i] << endl;
    }

private:
    vector<string> lines_;
};

// abstract command
class Command
{
public:
    Command(Document *doc) : doc_(doc) {}
    virtual void execute() = 0;
    virtual void unexecute() = 0;
    virtual void print() const = 0;
    virtual ~Command() {}

protected:
    Document *doc_;
};

// two concrete commands
class InsertCommand : public Command
{
public:
    InsertCommand(Document *doc, int line, const string &str) : Command(doc), line_(line), str_(str) {}
    void execute() override { doc_->insert(line_, str_); }
    void unexecute() override { doc_->remove(line_); }
    void print() const { cout << "Insert Line " << line_; }

private:
    int line_;
    string str_;
};

class EraseCommand : public Command
{
public:
    EraseCommand(Document *doc, int line) : Command(doc), line_(line), str_("") {}
    void execute() override { str_ = doc_->remove(line_); }
    void unexecute() override { doc_->insert(line_, str_); }
    void print() const { cout << "Erase Line " << line_; }

private:
    int line_;
    string str_;
};

// client
class DocumentWithHistory
{
public:
    DocumentWithHistory(const vector<string> &text = {}) : doc_(text) {}

    void insert(int line, string str)
    {
        Command *com = new InsertCommand(&doc_, line, str);
        com->execute();
        doneCommands_.push(com);
    }

    void erase(int line)
    {
        Command *com = new EraseCommand(&doc_, line);
        com->execute();
        doneCommands_.push(com);
    }

    void undo()
    {
        if (doneCommands_.size() != 0)
        {
            Command *com = doneCommands_.top();
            doneCommands_.pop();
            com->unexecute();
            delete com; // don't forget to delete command
        }
        else
            cout << "no commands to undo" << endl;
    }

    void show() { doc_.show(); }

    void printHistory()
    {
        std::stack<Command *> tmp;
        int i = 0;
        while (!doneCommands_.empty()) // go through commands
        {
            ++i;
            Command *currCommand = doneCommands_.top();
            doneCommands_.pop();
            cout << i << ": ";
            currCommand->print();
            cout << std::endl;
            tmp.push(currCommand);
        }
        while (!tmp.empty()) // resave them
        {
            Command *currCommand = tmp.top();
            tmp.pop();
            doneCommands_.push(currCommand);
        }
    }

    void redo(int choice)
    {
        if (doneCommands_.size() != 0 && choice < doneCommands_.size())
        {
            Command *currCommand;
            std::stack<Command *> tmp;
            for (int i = 0; i < choice; ++i)
            {
                currCommand = doneCommands_.top();
                doneCommands_.pop();
                tmp.push(currCommand);
            }
            currCommand->execute();
            for (int i = 0; i < choice - 1; ++i)
            {
                doneCommands_.push(tmp.top());
                tmp.pop();
            }
            doneCommands_.push(currCommand);
        }
        else
        {
            cout << "No commands to re-execute" << std::endl;
        }
    }

    class Memento *createMemento() const;
    void rollBack(class Memento *);

private:
    Document doc_;
    std::stack<Command *> doneCommands_;
};

class Memento
{
public:
    Memento(const DocumentWithHistory &doc) : doc_(doc) {}
    const DocumentWithHistory &getState() const { return doc_; }

private:
    const DocumentWithHistory doc_;
};

Memento *DocumentWithHistory::createMemento() const
{
    return new Memento(*this); // copying the Document itself
}

void DocumentWithHistory::rollBack(Memento *mem)
{
    *this = mem->getState(); // copying back
}

// invoker
int main()
{
    DocumentWithHistory his({"Lorem Ipsum is simply dummy text of the printing and typesetting",
                             "industry. Lorem Ipsum has been the industry's standard dummy text",
                             "ever since the 1500s, when an unknown printer took a galley of",
                             "type and scrambled it to make a type specimen book. It has",
                             "survived five centuries."});

    Memento *checkpoint = nullptr;
    char option;
    int line;
    string str;

    do
    {
        his.show();
        cout << endl;

        cout << "Enter option (i)nsert line, (e)rase line, (u)undo last command, (c)heckpoint, roll(b)ack, (h)history, (r)edo command: ";
        cin >> option;

        switch (option)
        {
        case 'i':
            cout << "line number to insert: ";
            cin >> line;
            cout << "line to insert: ";
            cin.get(); // removes end-of-line character so getline() works correctly
            getline(cin, str);
            his.insert(line, str);
            break;

        case 'e':
            cout << "line number to remove: ";
            cin >> line;
            his.erase(line);
            break;

        case 'u':
            his.undo();
            break;

        case 'c':
            checkpoint = his.createMemento();
            break;

        case 'b':
            if (checkpoint != nullptr)
            {
                his.rollBack(checkpoint);
                delete checkpoint;
            }
            break;

        case 'h':
            his.printHistory();
            break;

        case 'r':
            his.printHistory();
            cout << "Command number to redo: ";
            std::cin >> line;
            his.redo(line);
            break;
        }

    } while (option == 'i' || option == 'e' || option == 'u' || option == 'c' || option == 'b' || option == 'h' || option == 'r');
}