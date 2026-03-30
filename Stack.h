//***********************************************************
// Author: D.S. Malik
// This class specifies the basic operations on a stack as a
// linked list.
//***********************************************************

#ifndef STACK_H
#define STACK_H

#include <iostream>

class Stack
{
public:
    const Stack& operator=(const Stack&);
        //Overload the assignment operator.

    void initializeStack();
        //Function to initialize the stack to an empty state.
        //Postcondition: stackTop = nullptr;

    bool isEmptyStack() const;
        //Function to determine whether the stack is empty.
        //Postcondition: Returns true if the stack is empty,
        //               otherwise returns false.

    void push(int newItem);
        //Function to add newItem to the stack.
        //Precondition: The stack exists.
        //Postcondition: The stack is changed and newItem 
        //               is added to the top of the stack.

    int peek() const;
        //Function to return the top element of the stack.
        //Precondition: The stack exists and is not empty.
        //Postcondition: If the stack is empty, the program 
        //               terminates; otherwise, the top 
        //               element of the stack is returned.

    void pop();
        //Function to remove the top element of the stack.
        //Precondition: The stack exists and is not empty.
        //Postcondition: The stack is changed and the top 
        //               element is removed from the stack.

    Stack();
        //Default constructor
        //Postcondition: stackTop = nullptr;

    Stack(const Stack& otherStack);
        //Copy constructor

    ~Stack();
        //Destructor
        //Postcondition: All the elements of the stack are 
        //               removed from the stack.

private:
    struct nodeType
    {
        int info;
        nodeType *link;
    };

    nodeType *stackTop; //pointer to the stack

    void copyStack(const Stack& otherStack);
        //Function to make a copy of otherStack.
        //Postcondition: A copy of otherStack is created and
        //               assigned to this stack.
};

#endif