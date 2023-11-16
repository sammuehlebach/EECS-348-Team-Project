#include <stdexcept> //header for exception classes (new addition not mentioned in documents)
#include <cmath> //Header for math functions in C++

//Template definition for Arithmetic
template <typename T>
class Arithmetic {
private: //private variables
    T operand1; //First operand in operation
    T operand2; //Second operand in operation
    char op; //Operator in operation
    T result;

public:   
    //constructor that takes two operands and operator
    Arithmetic(T operand1, T operand2, char operation)
    : operand1(operand1), operand2(operand2), op(operation), result(0) {
        //initializes member variables
        switch (op) { //switch statement that runs methods based on
                        //inputted operation
            case '+': //if operation is '+'
                result = add(); //add method is called
                break; //breaks out of switch statement
            case '-': //if operation is '-'
                result = subtract(); //subtraction method is called
                break; //breaks out of switch statement
            case '*': //if operation is '*'
                result = multiply(); //multiplication method is called
                break; //breaks out of switch statment
            case '/': //if operation is '/'
                result = divide(); //divide method is called
                break; //breaks out of switch statement
            case '%': //if operation is '%;
                result = modulo(); ////modulo method is called
                break; //breaks out of switch statement
            case '^': //if operation is '^'
                result = exponent(); //exponent method is called
                break; //breaks out of switch statement
            default: //if none of these cases are met
                throw "Invalid operator";
                //in exception is thrown, indicating an invalid operator
        }
    }
    //method for addition
    T add() {
        return operand1 + operand2; //returns sum of operand1 and operand2
    }
    //method for subtraction
    T subtract() {
        return operand1 - operand2; //returns difference of operand1 and operand2
    }
    //method for multiplication
    T multiply() {
        return operand1 * operand2; //returns product of operand1 and operand2
    }
    //method for division
    T divide() {
        if (operand2 == 0)
            throw "Division by zero is not allowed.";
        return operand1 / operand2; //returns the quotient of operand1 and operand2
    }
    //method for modulo
    T modulo() {
        if constexpr (std::is_integral<T>::value) { //checks if type T is an integral type
            return operand1 % operand2; //if so, performs modulo
        } else {
            return static_cast<int>(operand1) % static_cast<int>(operand2);
            //otherwise it casts the operands to ints and performs modulo
        }
    }
    //method for exponentation
    T exponent() {
        return pow(operand1,operand2); //returns the result of taking the first
        //operand to the power of the second
    }

    T getResult() const {
    return result;
    }
};