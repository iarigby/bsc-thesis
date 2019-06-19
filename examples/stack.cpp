#include <iostream>
#include <cstdlib>
#include "stack.h"
// modified from https://www.techiedelight.com/stack-implementation-in-cpp/


// Constructor to initialize stack
template <typename T>
stack<T>::stack(int size)
{
    arr = new T[size];
    capacity = size;
    top = -1;
}

template <typename T>
stack<T>::~stack() {
    delete[] arr;
}
// helper function to communicate results since 
// can't get return types :D
template <typename T>
void stack<T>::toggleOutput(bool outPutMessages) {
    this->outPutMessages = outPutMessages;
}

// Utility function to add an element x in the stack
template <typename T>
void stack<T>::push(T x)
{
    if (isFull())
    {
        if (this->outPutMessages) {
            std::cout << "OverFlow\nProgram Terminated\n";
        }
	throw "OverFlow";
    }

    if (this->outPutMessages) {
        std::cout << "Inserting " << x << std::endl;
    }
    arr[++top] = x;
}

// Utility function to pop top element from the stack
template <typename T>
T stack<T>::pop()
{
    // check for stack underflow
    if (isEmpty())
    {
        if (this->outPutMessages) {
            std::cout << "UnderFlow\nProgram Terminated\n";
        }
        throw "Underflow";
    }

    if (this->outPutMessages) {
        std::cout << "pop: Removing " << peek() << std::endl;
}

    // decrease stack size by 1 and (optionally) return the popped element
    return arr[top--];
}

// Utility function to return top element in a stack
template <typename T>
T stack<T>::peek()
{
    if (!isEmpty()) {
        if (this->outPutMessages) {
            std::cout << "peek: returning " << arr[top] << std::endl; 
        }
        return arr[top];
    }
    else
        throw "empty stack";
}

// Utility function to return the size of the stack
template <typename T>
int stack<T>::size()
{
    return top + 1;
}

// Utility function to check if the stack is empty or not
template <typename T>
bool stack<T>::isEmpty()
{
    return top == -1;	// or return size() == 0;
}

// Utility function to check if the stack is full or not
template <typename T>
bool stack<T>::isFull()
{
    return top == capacity - 1;	// or return size() == capacity;
}

template class stack<int>;



