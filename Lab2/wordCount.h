#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>

class WordOccurrence
{
public:
    WordOccurrence(const std::string &word = "", int num = 0);
    bool matchWord(const std::string &); // returns true if word matches stored
    void increment();                    // increments number of occurrences
    std::string getWord() const;
    int getNum() const;
    bool operator<(WordOccurrence) const;

private:
    std::string word_;
    int num_;
};

class WordList
{
public:
    WordList(int size = 0);
    // add copy constructor, destructor, overloaded assignment
    WordList(const WordList &);
    ~WordList();
    WordList &operator=(WordList);

    // implement comparison as friend
    friend bool equal(const WordList &, const WordList &);

    void addWord(const std::string &);
    void print();
    void swap(WordList &);

private:
    WordOccurrence *wordArray_; // a dynamically allocated array of WordOccurrences
                                // may or may not be sorted
    int size_;
};