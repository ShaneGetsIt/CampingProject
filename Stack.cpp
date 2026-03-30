//***********************************************************
// Author: D.S. Malik
// This class specifies the basic operations on a stack as a
// linked list.
//***********************************************************

#include <iostream>
#include <cassert>
#include "Stack.h"

using namespace std;

//Default constructor
Stack::Stack()
{
    stackTop = nullptr;
}

//Destructor
Stack::~Stack()
{
    initializeStack();
}

//Function to determine whether the stack is empty.
bool Stack::isEmptyStack() const
{
    return (stackTop == nullptr);
}

//Function to initialize the stack to an empty state.
void Stack::initializeStack()
{
    nodeType *temp; //pointer to delete the node

    while (stackTop != nullptr) //while there are elements in the stack
    {
        temp = stackTop; //set temp to point to the current node
        stackTop = stackTop->link; //advance stackTop to the next node
        delete temp; //deallocate memory occupied by temp
    }
}

//Function to add newItem to the stack.
void Stack::push(int newItem)
{
    nodeType *newNode; //pointer to create the new node

    newNode = new nodeType; //create the node

    newNode->info = newItem; //store newItem in the node
    newNode->link = stackTop; //insert newNode before stackTop
    stackTop = newNode; //set stackTop to point to the top node
}

//Function to return the top element of the stack.
int Stack::peek() const
{
    assert(stackTop != nullptr); //if stack is empty, terminate the program

    return stackTop->info; //return the top element
}

//Function to remove the top element of the stack.
void Stack::pop()
{
    nodeType *temp; //pointer to deallocate memory

    if (stackTop != nullptr)
    {
        temp = stackTop; //set temp to point to the top node
        stackTop = stackTop->link; //advance stackTop to the next node
        delete temp; //delete the top node
    }
    else
        cout << "Cannot remove from an empty stack." << endl;
}

//Function to make a copy of otherStack.
void Stack::copyStack(const Stack& otherStack)
{
    nodeType *newNode, *current, *last;

    if (stackTop != nullptr) //if stack is nonempty, make it empty
        initializeStack();

    if (otherStack.stackTop == nullptr)
        stackTop = nullptr;
    else
    {
        current = otherStack.stackTop; //set current to point to the 
                                       //stack to be copied

        //copy the stackTop element of otherStack
        stackTop = new nodeType; //create the node

        stackTop->info = current->info; //copy the info
        stackTop->link = nullptr; //set the link field to nullptr
        last = stackTop; //set last to point to the node
        current = current->link; //set current to point to the next
                                 //node

        //copy the remaining stack
        while (current != nullptr)
        {
            newNode = new nodeType;

            newNode->info = current->info;
            newNode->link = nullptr;
            last->link = newNode;
            last = newNode;
            current = current->link;
        }//end while
    }//end else
}

//Copy constructor
Stack::Stack(const Stack& otherStack)
{
    stackTop = nullptr;
    copyStack(otherStack);
}

//Overload the assignment operator
const Stack& Stack::operator=(const Stack& otherStack)
{
    if (this != &otherStack) //avoid self-copy
        copyStack(otherStack);

    return *this;
}