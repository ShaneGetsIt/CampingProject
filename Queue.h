//***********************************************************
// Author: D.S. Malik
// This class specifies the basic operations on a queue as a
// linked list.
//***********************************************************

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

class Queue
{
public:
    const Queue& operator=(const Queue&);
        //Overload the assignment operator.

    void initializeQueue();
        //Function to initialize the queue to an empty state.
        //Postcondition: queueFront = nullptr; queueRear = nullptr;

    bool isEmptyQueue() const;
        //Function to determine whether the queue is empty.
        //Postcondition: Returns true if the queue is empty,
        //               otherwise returns false.

    void enqueue(int newItem);
        //Function to add newItem to the queue.
        //Precondition: The queue exists.
        //Postcondition: The queue is changed and newItem 
        //               is added to the rear of the queue.

    int frontElement() const;
        //Function to return the front element of the queue.
        //Precondition: The queue exists and is not empty.
        //Postcondition: If the queue is empty, the program 
        //               terminates; otherwise, the front 
        //               element of the queue is returned.

    void dequeue();
        //Function to remove the front element of the queue.
        //Precondition: The queue exists and is not empty.
        //Postcondition: The queue is changed and the front 
        //               element is removed from the queue.

    Queue();
        //Default constructor
        //Postcondition: queueFront = nullptr; queueRear = nullptr;

    Queue(const Queue& otherQueue);
        //Copy constructor

    ~Queue();
        //Destructor
        //Postcondition: All the elements of the queue are 
        //               removed from the queue.

private:
    struct nodeType
    {
        int info;
        nodeType *link;
    };

    nodeType *queueFront; //pointer to the front of the queue
    nodeType *queueRear;  //pointer to the rear of the queue

    void copyQueue(const Queue& otherQueue);
        //Function to make a copy of otherQueue.
        //Postcondition: A copy of otherQueue is created and
        //               assigned to this queue.
};

#endif