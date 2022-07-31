// Prototype scheduler
// Seth Winkhart
// 4/10/2022

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>

class State;

// individual processes
class Process
{
public:
    Process();
    Process(State *s) : state_(s), processID_(nextID_) { ++nextID_; }

    // state transitions to be called on a process object that inturn call the
    // appropriate state transiton of the current stored state object
    void admit();
    void dispatch();
    void suspend();
    void exit();
    void block();
    void unblock();

    int getProcessID() const { return processID_; }

    void report(std::ostream &);

    Process *clone()
    {
        Process *p = new Process(state_);
        return p;
    }

    void changeState(State *s) { state_ = s; }

private:
    static int nextID_;
    int processID_;
    State *state_;
};

// abstract state, contains state transitions to be implemented by states that
// are then called by the appropriate process state transition function on the
// current stored state
class State
{
public:
    virtual void admit(Process *) {}
    virtual void dispatch(Process *) {}
    virtual void suspend(Process *) {}
    virtual void exit(Process *) {}
    virtual void block(Process *) {}
    virtual void unblock(Process *) {}
    virtual void report(std::ostream &) = 0;

    void changeState(Process *p, State *s) { p->changeState(s); }
};

// creates a new process and moves it to the ready state
class NewProcess : public State
{
public:
    static State *instance()
    {
        static State *onlyInstance = new NewProcess;
        return onlyInstance;
    }

    void admit(Process *x) override;

    void report(std::ostream &out) override
    {
        out << "New" << std::endl;
    }

private:
    NewProcess() {}
};

// ready state, moves a process to the running state via dispatch
class Ready : public State
{
public:
    static State *instance()
    {
        static State *onlyInstance = new Ready;
        return onlyInstance;
    }

    void dispatch(Process *x) override;

    void report(std::ostream &out) override
    {
        out << "Ready" << std::endl;
    }

private:
    Ready() {}
};

// running state, moves a process to dead, blocked, or ready states with via
// suspend, exit, and block
class Running : public State
{
public:
    static State *instance()
    {
        static State *onlyInstance = new Running;
        return onlyInstance;
    }

    void exit(Process *x) override;
    void suspend(Process *x) override; // changes state to ready
    void block(Process *x) override;   // changes state to waiting

    void report(std::ostream &out) override
    {
        out << "Running" << std::endl;
    }

private:
    Running() {}
};

// waiting state, moves a process to the ready state with unblock
class Blocked : public State
{
public:
    static State *instance()
    {
        static State *onlyInstance = new Blocked;
        return onlyInstance;
    }

    void unblock(Process *x) override;

    void report(std::ostream &out) override
    {
        out << "Waiting" << std::endl;
    }

private:
    Blocked() {}
};

// kills a process, moves it to the Terminiated state
class Kill : public State
{
public:
    static State *instance()
    {
        static State *onlyInstance = new Kill;
        return onlyInstance;
    }

    void report(std::ostream &out) override
    {
        out << "Terminated" << std::endl;
    }

private:
    Kill() {}
};

// member definitions
// Process stuff
int Process::nextID_ = 0;

Process::Process()
{
    processID_ = nextID_;
    ++nextID_;
    state_ = NewProcess::instance();
}

void Process::admit() { state_->admit(this); }
void Process::dispatch() { state_->dispatch(this); }
void Process::suspend() { state_->suspend(this); }
void Process::exit() { state_->exit(this); }
void Process::block() { state_->block(this); }
void Process::unblock() { state_->unblock(this); }

void Process::report(std::ostream &out)
{
    out << "Process " << processID_ << " is currently ";
    state_->report(out);
}

// NewProcess stuff
void NewProcess::admit(Process *x) { x->changeState(Ready::instance()); }

// Ready stuff
void Ready::dispatch(Process *x) { x->changeState(Running::instance()); }

// Running stuff
void Running::exit(Process *x) { x->changeState(Kill::instance()); }
void Running::suspend(Process *x) { x->changeState(Ready::instance()); }
void Running::block(Process *x) { x->changeState(Blocked::instance()); }

// Blocked stuff
void Blocked::unblock(Process *x) { x->changeState(Ready::instance()); }

int main()
{
    srand(time(nullptr));

    const int numProcess = 5;
    Process prototype;

    std::queue<Process *> ready;
    std::queue<Process *> blocked;

    for (int i = 0; i != numProcess; ++i)
    {
        Process *temp = prototype.clone();
        temp->admit();
        temp->report(std::cout);
        ready.push(temp);
    }

    // make sure first process is running
    ready.front()->dispatch();
    ready.front()->report(std::cout);

    int decision;
    while (!ready.empty() || !blocked.empty())
    {
        // randomly decide if the current process should be terminated,
        // suspended, or blocked. 33% chance of each.
        if (!ready.empty())
        {
            decision = rand() % 3;
            switch (decision)
            {
            case 0: // exit and kill process
                ready.front()->exit();
                std::cout << "Process " << ready.front()->getProcessID() << " has terminiated." << std::endl;
                ready.pop();
                break;
            case 1: // block the process and make it wait
                ready.front()->block();
                blocked.push(ready.front());
                std::cout << "Process " << ready.front()->getProcessID() << " is waiting." << std::endl;
                ready.pop();
                break;
            case 2: // suspend the process and make it ready
                ready.front()->suspend();
                std::cout << "Process " << ready.front()->getProcessID() << " has been suspended." << std::endl;
                ready.push(ready.front());
                ready.pop();
                break;
            default:
                std::cout << "error" << std::endl;
                return 1;
            }
        }

        // randomly decide if the front element in the waiting queue should be
        // moved to ready. 50% chance.
        decision = rand() % 2;
        if (!blocked.empty() && decision == 0)
        {
            blocked.front()->unblock();
            ready.push(blocked.front());
            std::cout << "Process " << blocked.front()->getProcessID() << " is now ready." << std::endl;
            blocked.pop();
        }
    }
}
