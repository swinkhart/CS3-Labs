#include "wordCount.h"

//word occurence functions

//default and regular constructor using default parameters and initialization list
WordOccurrence::WordOccurrence(const std::string &word, int num) : word_(word), num_(num) {}

bool WordOccurrence::matchWord(const std::string &word)
{
    if (word != word_)
    {
        return false;
    }
    return true;
}

void WordOccurrence::increment()
{
    ++num_;
}

std::string WordOccurrence::getWord() const
{
    return word_;
}

int WordOccurrence::getNum() const
{
    return num_;
}

//overloaded less than operator for std::sort in addWord()
bool WordOccurrence::operator<(WordOccurrence rhs) const
{
    return num_ < rhs.getNum();
}

//WordList functions

//default and regular constructor using default parameters and initialization list
WordList::WordList(int size) : size_(size), wordArray_(size > 0 ? new WordOccurrence[size] : nullptr) {}

//copy constructor
WordList::WordList(const WordList &rhs) : size_(rhs.size_), wordArray_(rhs.size_ > 0 ? new WordOccurrence[rhs.size_] : nullptr)
{
    //std::cout << "copy consructor called" << std::endl;
    for (int i = 0; i < size_; ++i)
    {
        wordArray_[i] = rhs.wordArray_[i];
    }
}

//destructor
WordList::~WordList()
{
    //std::cout << "destructor called" << std::endl;
    delete[] wordArray_;
}

//overload assignment operator
WordList &WordList::operator=(WordList rhs)
{
    //std::cout << "overload assignment called" << std::endl;
    swap(rhs);
    return *this;
}

//check if the size and WordOccurences are the same
bool equal(const WordList &one, const WordList &two)
{
    if (one.size_ != two.size_)
    {
        return false;
    }

    for (int i = 0; i < one.size_ && i < two.size_; ++i)
    {
        if (one.wordArray_[i].getWord() != two.wordArray_[i].getWord() || one.wordArray_[i].getNum() != two.wordArray_[i].getNum())
        {
            return false;
        }
    }
    return true;
}

//check if there is already a word occurence for the word, if there is then
//increment the number of occurences for the WordOccurence, if not then make a
//new wordArray that is one larger than the old one, copy the contents of the
//old array into the new one, then at the end make a new WordOccurence. Finally
//delete the old wordArray, assign the new one to the member variable,
//incremnt the size then call sort to sort the array with rarest first.
void WordList::addWord(const std::string &word)
{
    for (int i = 0; i < size_; ++i)
    {
        if (wordArray_[i].matchWord(word))
        {
            wordArray_[i].increment();
            return;
        }
    }

    WordOccurrence *newWordArray = new WordOccurrence[size_ + 1];
    for (int i = 0; i < size_; ++i)
    {
        newWordArray[i] = wordArray_[i];
    }

    newWordArray[size_] = WordOccurrence(word, 1);

    delete[] wordArray_;
    wordArray_ = newWordArray;
    ++size_;

    std::sort(wordArray_, wordArray_ + size_);
}

//iterate through array and print word and the number of occurences
void WordList::print()
{
    for (int i = 0; i < size_; ++i)
    {
        std::cout << wordArray_[i].getWord() << ": " << wordArray_[i].getNum() << std::endl;
    }
}

//swap function for copy and swap idiom
void WordList::swap(WordList &rhs)
{
    int tmpSize_;

    tmpSize_ = rhs.size_;
    rhs.size_ = size_;
    size_ = tmpSize_;

    WordOccurrence *tmpWordArray_;

    tmpWordArray_ = rhs.wordArray_;
    rhs.wordArray_ = wordArray_;
    wordArray_ = tmpWordArray_;
}