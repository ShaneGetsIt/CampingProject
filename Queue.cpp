//***********************************************************
// Author: D.S. Malik
// This class specifies the basic operations on a queue as a
// linked list.
//***********************************************************

#include <iostream>
#include <cassert>
#include "Queue.h"

using namespace std;

//Default constructor
Queue::Queue()
{
    queueFront = nullptr;
    queueRear = nullptr;
}

//Destructor
Queue::~Queue()
{
    initializeQueue();
}

//Function to determine whether the queue is empty.
bool Queue::isEmptyQueue() const
{
    return (queueFront == nullptr);
}

//Function to initialize the queue to an empty state.
void Queue::initializeQueue()
{
    nodeType *temp; //pointer to delete the node

    while (queueFront != nullptr) //while there are elements in the queue
    {
        temp = queueFront; //set temp to point to the current node
        queueFront = queueFront->link; //advance queueFront to the next node
        delete temp; //deallocate memory occupied by temp
    }

    queueRear = nullptr; //set queueRear to nullptr
}

//Function to add newItem to the queue.
void Queue::enqueue(int newItem)
{
    nodeType *newNode; //pointer to create the new node

    newNode = new nodeType; //create the node

    newNode->info = newItem; //store newItem in the node
    newNode->link = nullptr; //set the link field to nullptr

    if (queueFront == nullptr) //if initially the queue is empty
    {
        queueFront = newNode;
        queueRear = newNode;
    }
    else //add newNode at the end
    {
        queueRear->link = newNode;
        queueRear = queueRear->link;
    }
}

//Function to return the front element of the queue.
int Queue::frontElement() const
{
    assert(queueFront != nullptr); //if queue is empty, terminate the program

    return queueFront->info; //return the front element
}

//Function to remove the front element of the queue.
void Queue::dequeue()
{
    nodeType *temp; //pointer to deallocate memory

    if (queueFront != nullptr)
    {
        temp = queueFront; //set temp to point to the front node
        queueFront = queueFront->link; //advance queueFront to the next node
        delete temp; //delete the front node

        if (queueFront == nullptr) //if after deletion the queue is empty
            queueRear = nullptr; //set queueRear to nullptr
    }
    else
        cout << "Cannot remove from an empty queue." << endl;
}

//Function to make a copy of otherQueue.
void Queue::copyQueue(const Queue& otherQueue)
{
    nodeType *newNode, *current, *last;

    if (queueFront != nullptr) //if queue is nonempty, make it empty
        initializeQueue();

    if (otherQueue.queueFront == nullptr)
    {
        queueFront = nullptr;
        queueRear = nullptr;
    }
    else
    {
        current = otherQueue.queueFront; //set current to point to the 
                                         //queue to be copied

        //copy the queueFront element of otherQueue
        queueFront = new nodeType; //create the node

        queueFront->info = current->info; //copy the info
        queueFront->link = nullptr; //set the link field to nullptr
        last = queueFront; //set last to point to the node
        queueRear = queueFront; //set queueRear to point to the node
        current = current->link; //set current to point to the next
                                 //node

        //copy the remaining queue
        while (current != nullptr)
        {
            newNode = new nodeType;

            newNode->info = current->info;
            newNode->link = nullptr;
            last->link = newNode;
            last = newNode;
            current = current->link;
        }//end while

        queueRear = last; //set queueRear to point to the last node
    }//end else
}

//Copy constructor
Queue::Queue(const Queue& otherQueue)
{
    queueFront = nullptr;
    queueRear = nullptr;
    copyQueue(otherQueue);
}

//Overload the assignment operator
const Queue& Queue::operator=(const Queue& otherQueue)
{
    if (this != &otherQueue) //avoid self-copy
        copyQueue(otherQueue);

    return *this;
}