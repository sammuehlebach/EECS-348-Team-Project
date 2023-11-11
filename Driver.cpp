// Driver.cpp
// EECS 348
// Fall 2023
// Sam Muehlebach, Josh Welicky, Jennifer Aber, Jawad Ahsan, Basim Arshad, Mark Kitchin

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>

//#include "infixcalc.h"
#include "theStack.h"
#include "aVector.h"

using namespace std;

//function declarations
void tokenize(const std::string& s, MyVector<std::string>& tokens);
int operatorPrec(const char c);
bool isValidParenthesis(const char c);
bool isDigit(const char c);
//void convert(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens);

int main()
{
        //run function - goes through all the main functions needed to calculate a result
        string user_input;
        cout << "Enter an expression: ";
        getline(cin, user_input);
        MyVector<std::string> myinfixtokens;
        MyVector<std::string> myposttokens;
        tokenize(user_input, myinfixtokens);
        
    return 0;
}

//function to keep track of operator precedence.  Lower numbers have higher precedence. 
int operatorPrec(const char c)
{
    switch (c)
    {
    case '*':
        return 2;
    case '/':
        return 2;
    case '+':
        return 3;
    case '-':
        return 3;
    default:
        return -1;
    }
}

// checks for parentheses in the input.  
bool isValidParenthesis(const char c)
{
    switch (c)
    {
    case '(':
        return true;
    case ')':
        return true;
    default:
        return false;
    }
}

// checks if a character corresponds to a valid digit.
bool isDigit(const char c)
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
}


// tokenizes an infix string s into a set of tokens (operands or operators)
// handles operands that are negative numbers, operators, and parentheses. The bool "AssociateSign" is 
//to keep track of instances where a double negative is entered i.e. 3 - - 3.  In a case where AssociateSign is
//True, two characters are pushed on to the tempstring, creating a token with an operand with the - sign
//before.  
void tokenize(const std::string& s, MyVector<std::string>& tokens)
{
    // code begins

    bool AssociateSign = true;
    std::string tempString = "";

    for (unsigned int i = 0; i < s.length(); i++)
    {
        //pushes digits onto a temporary string until a parenthesis or operator is found
        if (operatorPrec(s[i]) < 0 && !isValidParenthesis(s[i]))
        {
            while (operatorPrec(s[i]) < 0 && !isValidParenthesis(s[i]) && (i < s.length()))
            {   //removes spaces
                if (isDigit(s[i])) {
                    tempString = tempString + s[i];
                }

                i++;
            }

            if (tempString.length() > 0) {
                tokens.push_back(tempString);
            }

            tempString = "";
            AssociateSign = false;
        }
        //if s[i] is an operator and the AssociateSign flag is true 
        if (operatorPrec(s[i]) > 0 && AssociateSign)
        {
            //pushes operator onto the temp string
            tempString = tempString + s[i];
            AssociateSign = false;
            i++;
            //pushes operand(s) onto temp string
            while (operatorPrec(s[i]) < 0 && !isValidParenthesis(s[i]) && (i < s.length()))
            {
                tempString = tempString + s[i];
                i++;
            }

            //adds to infix vector
            tokens.push_back(tempString);
            tempString = "";
        }

        //for operators with AssociateSign = false
        if ((operatorPrec(s[i]) > 0 && !AssociateSign))
        {
            tempString = tempString + s[i];
            tokens.push_back(tempString);
            tempString = "";
            if (operatorPrec(s[i + 1]) > 0) {
                AssociateSign = true;
            }
        }

        //open parenthesis, showing the beginning of an expression.  If the character right after is is
        //an operator, set the AssociateSign to true. i.e. (-3 + 3) 
        if (s[i] == '(')
        {
            tempString = tempString + s[i];
            tokens.push_back(tempString);
            tempString = "";
            if (operatorPrec(s[i + 1]) > 0) {
                AssociateSign = true;
            }
        }
        
        //end parenthesis, marking the end of an expression
        if (s[i] == ')')
        {
            tempString = tempString + s[i];
            tokens.push_back(tempString);
            tempString = "";
        }

    }  //END For Loop

    //prints out tokens (for testing purposes only)
    for (int i = 0;  i < tokens.size(); i++)

    {
        cout << tokens[i];
        cout << " ";
    }

    // code ends
}

//function start for convert
void convert(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens) {}