// aVector.h
// EECS 348
// Fall 2023
// Sam Muehlebach, Josh Welicky, Jennifer Aber, Jawad Ahsan, Basim Arshad, Mark Kitchin


#ifndef __MYVECTOR_H__
#define __MYVECTOR_H__

#include <utility>
#include <vector>

//aVector.h uses a template, since each Vector can store various types of objects (i.e. double, char)
//theStack.h inherits from Vector.h.

//The Vector class is used by several methods in the Arithmetic class to store and 
//access operators and operands.  
template <typename DataType>
class MyVector
{
protected:
    /* data */
    int theSize;     // the number of data elements the vector currently holds
    int theCapacity; // maximum data elements the vector can hold
    DataType* data;     // address of the data storage

public:

    static const int SPARE_CAPACITY = 16;   // initial capacity of the vector

    // default constructor
    // The constructor initializes the vector with a size of 0, and a capacity = 16.  
    // The vector initially contains an array of DataType elements. 
    explicit MyVector(int initSize = 0) :
        theSize{ initSize },
        theCapacity{ initSize + SPARE_CAPACITY }
    {
        
        data = new DataType[theCapacity];

    }  // END Default Constructor

    // copy constructor - i.e. MyVector copy = oldVector
    // The copy constructor creates a copy of the Vector object with identical size, capacity, and data
    // as the original.  It uses a for loop to copy over the data.  
    MyVector(const MyVector& rhs) :
        theSize{ rhs.theSize },
        theCapacity{ rhs.theCapacity }
    {  

        data = new DataType[theCapacity];
        for (int i = 0; i < theSize; ++i)
        data[i] = rhs.data[i];

    }  

    // move constructor - works with std::move
   // creates a copy of rhs (right-hand side) and deletes the original.  
    MyVector(MyVector&& rhs) :
        theSize{ rhs.theSize },
        theCapacity{ rhs.theCapacity },
        data{ rhs.data }
    {

        rhs.data = nullptr;
        rhs.theSize = 0;
        rhs.theCapacity = 0;

    }


    // destructor
    ~MyVector() 
    {

        delete[] data;

    };

   

    // changes the size of the array
    //If a vector object is created with a size greater than the default capacity, the resize function
    //doubles the capacity via the reserve function (see below) and sets the size = newSize
    void resize(int newSize)
    {
        
        if (newSize > theCapacity)
            reserve(newSize * 2);

        theSize = newSize;

    }

    // The reserve function creates a pointer to a new, empty array of size newCapacity used 
    // to hold the data. It uses a for loop to copy the data from the old Vector to the new, larger array 
    // (newdata) then uses the swap function to update the data attribute with the new data.  It then updates
    // the capacity attribute. 
    void reserve(int newCapacity)
    {
        
        DataType* newdata = new DataType[newCapacity];

        if (newCapacity < theSize)
            return;

        for (int i = 0; i < theSize; ++i)
            newdata[i] = std::move(data[i]);

        std::swap(data, newdata);

        delete[] newdata;

        theCapacity = newCapacity;

    }

    //operator overload - allows access of elements in the vector by index.
    //because data is a protected attribute, this allows member clases (such as Stack) to access the data
    const DataType& operator[](int index) const
    {
        
        return data[index];

    }


    // check if the vector is empty; return TRUE if the vector is empty
    bool empty() const
    {
       
        return size() == 0;

    }

    // returns the size of the vector
    int size() const
    {
       
        return theSize;

    }

    // returns the capacity of the vector
    int capacity() const
    {

        return theCapacity;

    }

    // insert an data element to the end of the vector
    void push_back(const DataType& x)
    {
     
        if (theSize == theCapacity)
        {

            reserve(2 * theCapacity + 1);

        }
        
        data[theSize++] = x;

        // code ends
    }

   

    // remove the last data element from the array
    void pop_back()
    {
        
        --theSize;

    }

    // returns the last data element from the array
    const DataType& back() const
    {
        
        return data[theSize - 1];

    }

};


#endif // __MYVECTOR_H__
