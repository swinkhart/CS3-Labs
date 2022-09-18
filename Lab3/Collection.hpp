#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include "list.hpp"

template <typename T>
class Collection;

template <typename T>
bool equal(const Collection<T> &, const Collection<T> &);

template <typename T>
class Collection
{
public:
    Collection() : head_(nullptr) {}
    void add(const T &item);
    void remove(const T &item);
    T last();
    void print();
    friend bool equal<T>(const Collection &, const Collection &);

private:
    node<T> *head_;
};

// function definitions here because template

template <typename T>
void Collection<T>::add(const T &item)
{
    if (head_ == nullptr)
    {
        head_ = new node<T>();
        head_->setData(item);
    }
    else
    {
        node<T> *temp = new node<T>();
        temp->setData(item);
        node<T> *current = head_;
        while (current->getNext() != nullptr)
        {
            current = current->getNext();
        }
        current->setNext(temp);
    }
}

template <typename T>
void Collection<T>::remove(const T &item)
{
    if (head_ == nullptr)
    {
        return;
    }
    else
    {
        node<T> *current = head_;
        while (head_->getData() == item)
        {
            head_ = head_->getNext();
            delete current;
            current = head_;
            if (head_ == nullptr)
            {
                return;
            }
        }
        node<T> *prev = head_;
        current = current->getNext();
        while (current != nullptr)
        {
            while (current != nullptr && current->getData() == item)
            {
                prev->setNext(current->getNext());
                delete current;
                current = prev->getNext();
            }
            if (current == nullptr)
            {
                break;
            }
            prev = current;
            current = current->getNext();
        }
    }
}

template <typename T>
void Collection<T>::print()
{
    node<T> *current = head_;
    for (current = head_; current != nullptr; current = current->getNext())
    {
        std::cout << current->getData() << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
bool equal(const Collection<T> &one, const Collection<T> &two)
{
    node<T> *currentOne = one.head_;
    node<T> *currentTwo = two.head_;
    while ((currentOne->getNext() != nullptr) && (currentOne->getData() == currentTwo->getData()))
    {
        currentOne = currentOne->getNext();
        currentTwo = currentTwo->getNext();
    }
    return (currentOne->getData() == currentTwo->getData());
}
#endif