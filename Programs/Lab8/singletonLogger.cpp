// singleton file editor
// Seth Winkhart
// 03/19/2022

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

class Logger
{
public:
    static Logger *instance();
    void report(const std::string &);

private:
    // protected against the client and avoids copying
    Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static void cleanUp();

    // pointer to instance
    static Logger *onlyInstance_;
};

// function definitions

Logger *Logger::onlyInstance_ = nullptr;

Logger *Logger::instance()
{
    if (onlyInstance_ == nullptr)
    {
        onlyInstance_ = new Logger;
    }
    return onlyInstance_;
}

void Logger::report(const std::string &str)
{
    std::ofstream fout;
    std::string fileName;

    fout.open("log.txt ", std::fstream::out | std::fstream::app);
    if (fout.is_open())
    {
        fout << str << std::endl;
    }

    fout.close();
}

void Logger::cleanUp()
{
    delete onlyInstance_;
    onlyInstance_ = nullptr;
}

Logger::Logger() { atexit(cleanUp); }

// main function
int main()
{
    std::string str = "this is a test string";

    Logger::instance()->report(str);
}