
// Driver.cpp
// EECS 348
// Fall 2023
// Sam Muehlebach, Josh Welicky, Jennifer Aber, Jawad Ahsan, Basim Arshad, Mark Kitchin

#include <iostream>
#include <string>
#include "theStack.h"
#include "aVector.h"
#include "Arithmetic.h"

using namespace std;

class Driver {
public:
    Driver();
    void run();

private:
    string user_input;
    int operatorPrec(const char);
    bool isDigit(const char);
    void parse(const std::string&, MyVector<std::string>&);
    void evaluate(MyVector<std::string>&);
    void convert(MyVector<std::string>&, MyVector<std::string>&);
    double solve(MyVector<std::string>&);
};


Driver::Driver () {user_input = "";}

//function to keep track of operator precedence.  Lower numbers have higher precedence. 
int Driver::operatorPrec(const char c)
{
    switch (c)
    {
        case '^':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 3;
        case '(':
        case ')':
            return 0;
        default:
            return -1;
    }
}

// checks if a character corresponds to a valid digit.
bool Driver::isDigit(const char c)
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
}


// tokenizes an infix string s into a set of tokens (operands or operators)
// handles operands that are negative numbers, operators, and parentheses. Note that
//unary operators (+ or -) are tokenized with their respective numbers. In the instance where
//a unary operator is associated with a parenthesis, it is tokenized as a binary operator. Later
//the result of the parenthetical expression will added to or subtracted from 0.
void Driver::parse(const std::string& s, MyVector<std::string>& tokens)
{
    // code begins
    //each iteration, tempString will be constructed into a token, added to tokens, and reset to an empty string.
    std::string tempString = "";

    for (unsigned int i = 0; i < s.length(); i++)
    {
        //pushes digits onto a temporary string until an operator or space is found.
        if (operatorPrec(s[i]) < 0)
        {
            while (operatorPrec(s[i]) < 0 && (i < s.length()) && s[i] != ' ')
            {   
                //Checks to make sure the character is either a digit or decimal point. If not, an exception is thrown to run().
                if (isDigit(s[i]) || s[i] == '.') {
                    tempString = tempString + s[i];
                } else {
                    throw "Invalid Character Detected";
                }

                i++;
            }
            if (tempString.length() > 0) {
                tokens.push_back(tempString);
            }
            tempString = "";
        }

        //If/else block that processes operators.
        //This first block processes + and -. These have the potential to be unary operators (positive/negative signs).
        if (operatorPrec(s[i]) >= 0 && (s[i] == '+' || s[i] == '-')) 
        {
            tempString = tempString + s[i];
            //This checks if a digit or decimal point is immediatey next to it. Inputs like -.75 will be permitted.
            if ((i + 1 < s.length()) && operatorPrec(s[i+1]) < 0)
            {
                //If there is no number immediately before it, it is a unary operator. It will be added to tempString and tokenized with its associated number.
                if (i == 0 || (operatorPrec(s[i-1]) >= 0 && s[i-1] != ')'))
                    continue;
                
                //If there isn't, it's a standard + or - sign and is tokenized independently.
                else 
                    tokens.push_back(tempString);
                    tempString = "";
            //This runs if there is no digit/decimal immediately next to the plus or minus sign (it's simply addition/subtraction). It will be tokenized as a binary operator.
            } else {
                tokens.push_back(tempString);
                tempString = "";
            }
            
        //This covers the tokenization of the other operators except for *, which will need to be processed in special manner.
        } else if (operatorPrec(s[i]) >= 0 && s[i] != '*') 
        {
            tempString = tempString + s[i];
            tokens.push_back(tempString);
            tempString = "";
        }

        //This statement covers the tokenization of the * key. Note that the exponent operator can be denoted by ** as well as ^.
        if (s[i] == '*') {
            //Checks if the next character is a *, marking exponentiation. Tokenizes an ^ character.
            if ((i+1 < s.length()) && (s[i+1] == '*')) {
                tempString = tempString + '^';
                tokens.push_back(tempString);
                tempString = "";
            //This is the follow up. If the previous character is *, then exponentiation has already been tokenized, and this character should be ignored.
            } else if ((i-1 >= 0) && (s[i-1] == '*')) {
                continue;
            //This handles standard multiplication. Tokenizes the * as the * character.
            } else {
                tempString = tempString + s[i];
                tokens.push_back(tempString);
                tempString = "";
            }
        }
        //This conditional covers an instance like (-(3)). Here, a zero is tokenized, converting the expression to (0 - (3)).
        if (s[i] == '(' && i+1 < s.length() && operatorPrec(s[i+1]) == 3 && i+2 < s.length() && s[i+2] == '(')
            tokens.push_back("0");

    }  //END For Loop
    /*
    //Prints out all infix tokens for testing purposes.
    for (int i = 0; i < tokens.size(); i++)
        cout << tokens[i] << ' ';
    */
    // code ends
}


/*This is the convert function. It takes in an vector of tokens ordered in infix notation (3+4) and 
Builds up another vector of tokens reordered into postfix notation (3 4 +).*/
void Driver::convert(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens) {
    MyStack<std::string> op_stack;

    //This checks for incidents where an expression starts with something like -(3), where the unary operator is tokenized as a binary operator.
    //This will add a 0 as a post fix token. As an example, -(3) = 0-(3) = 0 3 -.
    if (operatorPrec(infix_tokens[0][0]) == 3) {
        std::string start = "0";
        postfix_tokens.push_back(start);
    }

    //This for loop will iterate through every infix token, reordering them into postfix notation.
    for (int i = 0; i < infix_tokens.size(); i++) {
        //This if statement covers numbers. They should be added to postfix tokens as they are encountered.
        if (infix_tokens[i].length() > 1 || operatorPrec(infix_tokens[i][0]) < 0)
            postfix_tokens.push_back(infix_tokens[i]);
        
        //This covers operators.
        else {
            //This if statement covers the operators that aren't parentheses.
            if (operatorPrec(infix_tokens[i][0]) > 0) {
                //If the operator stack is empty, the operator should be stored.
                if (op_stack.empty())
                    op_stack.push(infix_tokens[i]);
                //If there are operators on the stack, the current token's precedence will need to be compared against the top.
                else {
                    //If the current token has higher precedence*closer to zero), it should be pushed onto the stack.
                    if (operatorPrec(infix_tokens[i][0]) < operatorPrec(op_stack.peek()[0]))
                        op_stack.push(infix_tokens[i]);
                    else {
                        //If not, the top of the stack should be removed and added to postfix_tokens until the current token has the same or greater precedence and can be added to the stack.
                        while (!op_stack.empty() && operatorPrec(infix_tokens[i][0]) >= operatorPrec(op_stack.peek()[0]) && operatorPrec(op_stack.peek()[0]) != 0) {
                            postfix_tokens.push_back(op_stack.peek());
                            op_stack.pop();
                        }
                        op_stack.push(infix_tokens[i]);
                    }
                }
            //This covers parentheses.
            } else {
                //Opening parentheses should be added to the stack as encountered.
                if (infix_tokens[i][0] == '(')
                    op_stack.push(infix_tokens[i]);
                //When a closing parentheses is found, the operators on the stack should be removed and added to postfix_tokens until the opening parentheses has been found. After which it should be removed.
                else {
                    while (!op_stack.empty() && op_stack.peek()[0] != '(') {
                        postfix_tokens.push_back(op_stack.peek());
                        op_stack.pop();
                    }  
                    if (!op_stack.empty())
                        op_stack.pop();
                }
            }
        }
    }
    //End for loop.
    //All remaining operators on the stack should be removed and placed in postfix_tokens.
    while (!op_stack.empty()) {
        postfix_tokens.push_back(op_stack.peek());
        op_stack.pop();
    }

    /*
    //Prints all postfix tokens for testing purposes.
    for (int i = 0; i < postfix_tokens.size(); i++)
        cout << postfix_tokens[i] << ' ';

    cout << endl;
    */
}



/*This is the evaluate function. It handles the evaluation of user input. It takes in a vector of tokens and analyzes each 
token one by one. To detect improperly ordered tokens, the prior token is often checked. A further description of the criteria
for each kind of token is displayed below.*/
void Driver::evaluate(MyVector<std::string>& tokens) {
    //Accounts for instance where the user places in no input.
    if (tokens.size() == 0)
        throw "No input detected.";

    //These keep track of how many open/closed parentheses are found. Will be analyzed at the end.
    int open_parentheses = 0;
    int closed_parentheses = 0;

    //For loop iterates through every token.
    for (int i = 0; i < tokens.size(); i++)
    {
        //This block checks large tokens, which can only be numbers.
        if (tokens[i].length() > 1) 
        {
            int decimal_points = 0;
            for (int j = 0; j < tokens[i].length(); j++)
            {
                if (tokens[i][j] == '.')
                    decimal_points++;
            }
            //After the for loop counts how many decimal points are present, an error is thrown if more than one are found.
            if (decimal_points > 1)
                throw "Multiple decimal points detected in one number.";

            //This covers an instance where a missing operator occurs, such as 3 -4  or (2+3)4.
            if (i != 0 && (tokens[i-1].length() > 1 || (operatorPrec(tokens[i-1][0]) < 1 && tokens[i-1][0] != '(')))
                throw "A number can only be preceded by a binary operator or an opening parentheses.";
        
        //This block checks single character tokens.
        } else {
            int precedence = operatorPrec(tokens[i][0]);

            //This block checks numbers. It covers instances like the inputting of a single "." and checks for missing operators like above.
            if (precedence < 0) {
                if (tokens[i][0] == '.')
                    throw "Invalid decimal point detected.";
                if (i != 0 && (tokens[i-1].length() > 1 || (operatorPrec(tokens[i-1][0]) < 1 && tokens[i-1][0] != '(')))
                    throw "A number can only be preceded by a binary operator or an opening parentheses.";
            
            //This block checks the arithmetic operators that aren't parentheses. 
            } else if (precedence > 0) {
                
                //Note that +(3) is a legal input.
                if (i == 0 && operatorPrec(tokens[i][0]) == 3)
                    continue;
                //Non-unary operators at the beginning are not allowed. Note that this is an incidence where two binary operators are next to each other.
                if (i == 0 || (tokens[i-1].length() == 1 && (operatorPrec(tokens[i-1][0]) > 0 || tokens[i-1][0] == '(')))
                    throw "A binary operator can only be preceded by a number or closing parentheses.";
                //Stray operators at the end of the expression are not allowed.
                if (i + 1 == tokens.size())
                    throw "A binary operator must be followed by a number or opening parentheses.";
            
            } else {
                //Open parentheses are counted. In addition, they have to be preceded by either another opening parentheses or an operator, not a number.
                if (tokens[i][0] == '(') {
                    open_parentheses++;
                    if (i != 0 && (tokens[i-1].length() > 1 || (operatorPrec(tokens[i-1][0]) < 1 && tokens[i-1][0] != '(')))
                        throw "An opening parentheses can only be preceded by an operator or an opening parentheses.";
                //Closing parentheses are counted. They must be preceded by a number or another closing parentheses.
                } else {
                    closed_parentheses++;
                    if (i == 0 || (tokens[i-1].length() == 1 && (operatorPrec(tokens[i-1][0]) > 0 || tokens[i-1][0] == '(')))
                        throw "A closed parentheses can only be preceded by a number or closing parentheses.";
                }
                    
            }
        }   
    }
    //Finally, there must be an equal number of opening and closing parentheses.
    if (open_parentheses != closed_parentheses)
        throw "Unmatched parentheses detected.";
    
    //Short output statement for testing purposes.
    //cout << "Evaluated" << endl;
}



/*This is the solve function. It takes in a vector full of tokens, ordered in postfix format. It divides the expression
into individual components by utilizing a Stack. The operands are placed onto the stack until their respective operator is 
found. Then, they are removed from the stack, and the result of their operation is then added to the stack. This operation
may then be utilized in another operation until the final result has been found and returned.*/
double Driver::solve(MyVector<std::string>& tokens) {
    //This stack will store all of the operands(numbers), as well as the result of subexpressions.
    MyStack<double> result_stack;

    for (int i = 0; i < tokens.size(); i++) {
        //Numerical tokens are cast into doubles and added to the stack as encountered.
        if (tokens[i].length() > 1 || operatorPrec(tokens[i][0]) < 0) {
            double operand = std::stod(tokens[i]);
            result_stack.push(operand);
        //When an operator is found, the top two operands are removed from the stack. The operands and operator are passed to an instance of the Arithmetic
        //class. Their result is then added to the stack.
        } else {
            char op = tokens[i][0];
            double operand2 = result_stack.peek();
            result_stack.pop();
            double operand1 = result_stack.peek();
            result_stack.pop();
            Arithmetic<double> result (operand1, operand2, op);
            result_stack.push(result.getResult());
        }
    }
    //The final result of the expression will be at the top of the stack. It is returned.
    return result_stack.peek();
}


/*This is the run() method. It handles all interaction with the user and makes all necessary function
calls to convert a user's input into a vector of infix tokens, evaluate those tokens for invalid input,
convert the infix tokens into a vector of postfix tokens for easy analysis, and the calculation and displaying
of the ultimate result of the expression.*/
void Driver::run() {
    //Obtains an expression from the user.
    cout << "Enter STOP to terminate program.\nEnter an expression: ";
    getline(cin, user_input);
    
    //This will run until the user inputs the message "STOP".
    while (user_input != "STOP") {
        //Empty vectors that will contain the infix and postfix tokens respectively.
        MyVector<std::string> infix_tokens;
        MyVector<std::string> postfix_tokens;

        try {
            //Parses the input into infix tokens, evaluates the tokens for invalid input, converts to postfix ordering, and calculates/prints the solution.
            parse(user_input, infix_tokens);
            evaluate(infix_tokens);
            convert(infix_tokens, postfix_tokens);
            double result = solve(postfix_tokens);
            cout << result << endl;
        }
        //This catch block ensures that whatever error message that has been thrown is printed to the screen.
        catch (const char* message) {
            cout << message << endl;
        }
        //Input is reobtained from the user.
        cout << "\nEnter STOP to terminate program.\nEnter an expression: ";
        getline(cin, user_input);

    }
        
    cout << "Program terminating..." << endl;
}

//Very minimal main() function, which simply creates an instance of Driver and starts the run() method.
int main()
{
    Driver driver = Driver();
    driver.run();  
    return 0;
}
