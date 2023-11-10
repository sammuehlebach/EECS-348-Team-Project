// theStack.h
// EECS 348
// Fall 2023
// Sam Muehlebach, Josh Welicky, Jennifer Aber, Jawad Ahsan, Basim Arshad, Mark Kitchin

#pragma once
#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include <iostream>
#include <algorithm>
#include "aVector.h"

// The Stack class inherits much of its functionality from the Vector class, but is designed to work with 
// LIFO (Last In First Out).  New items will be added to the top of the stack using push_back(), and items 
// on the top of the stack are removed using pop().  The top of the stack is the most recently added element.

template <typename DataType>
class MyStack : private MyVector<DataType>
{
public:

    // default constructor 
    explicit MyStack(int initSize = 0) :MyVector<DataType>{ initSize }
    {
        //The constructors and destructors are contained in the Vector class. 
    }

    // copy constructor 
    MyStack(const MyStack& rhs) :MyVector<DataType>{ rhs }
    {
    }

    // move constructor
    MyStack(MyStack&& rhs) :MyVector<DataType>{ rhs }
    {
    }

    // destructor
    ~MyStack()
    {
        // MyVector<DataType>::~MyVector();
    }

    // insert x to the stack
    void push(const DataType& x)
    {

        MyVector<DataType>::push_back(x);

    }

    // remove the last element from the stack
    void pop(void)
    {

        MyVector<DataType>::pop_back();

    }

    // access the last element of the stack
    const DataType& peek(void) const
    {

        return MyVector<DataType>::back();

    }

    // check if the stack is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {

        return MyVector<DataType>::empty();

    }

    // access the size of the stack
    int size() const
    {
        
        return MyVector<DataType>::size();

    }

    // access the capacity of the stack
    int capacity(void) const
    {
        
        return MyVector<DataType>::capacity();

    }

};

#endif // __MYSTACK_H__
