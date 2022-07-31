// state transitions
// Seth Winkhart
// 4/8/2022

#include <iostream>
#include <string>

class State;

// individual processes
class Process
{
public:
    Process();

    // state transitions to be called on a process object that inturn call the
    // appropriate state transiton of the current stored state object
    void create();
    void dispatch();
    void suspend();
    void exit();
    void block();
    void unblock();

    void report(std::ostream &);

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
    virtual void create(Process *) {}
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

    void create(Process *x) override;

    void report(std::ostream &out) override
    {
        out << "New" << std::endl;
    }

private:
    // NewProcess() {}
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
    // Ready() {}
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
    // Running() {}
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
    // Blocked() {}
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
    // Kill() {}
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

void Process::create() { state_->create(this); }
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
void NewProcess::create(Process *x) { x->changeState(Ready::instance()); }

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
    Process proc1;

    proc1.report(std::cout);
    proc1.create();
    proc1.report(std::cout);
    proc1.dispatch();
    proc1.report(std::cout);

    Process proc2;

    proc1.block();
    proc1.report(std::cout);
    proc2.create();
    proc2.report(std::cout);
    proc2.dispatch();
    proc2.report(std::cout);
    proc1.unblock();
    proc1.report(std::cout);
    proc2.exit();
    proc2.report(std::cout);

    proc1.dispatch();
    proc1.report(std::cout);
    proc1.suspend();
    proc1.report(std::cout);
    proc1.dispatch();
    proc1.report(std::cout);
    proc1.exit();
    proc1.report(std::cout);
}