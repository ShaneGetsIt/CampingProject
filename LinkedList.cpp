#pragma once

#ifndef LINKEDLIST_CPP
#define LINKEDLIST_CPP

#include "LinkedList.h"

// ============================================
// CONSTRUCTORS AND DESTRUCTOR
// ============================================

// Default constructor - creates empty list
template <typename T>
LinkedList<T>::LinkedList()
    : first(nullptr), last(nullptr), count(0)
{
    // All pointers initialized to nullptr, count to 0
}

// Copy constructor - creates deep copy of another list
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other)
    : first(nullptr), last(nullptr), count(0)
{
    copyList(other);  // Use helper function to copy all nodes
}

// Destructor - deletes all nodes to prevent memory leaks
template <typename T>
LinkedList<T>::~LinkedList()
{
    clear();  // Delete all nodes
}

// ============================================
// OVERLOADED OPERATORS
// ============================================

// Assignment operator - deep copy with self-assignment check
template <typename T>
const LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
    if (this != &other)  // Avoid self-assignment (list = list)
    {
        clear();         // Delete existing nodes
        copyList(other); // Copy nodes from other list
    }
    return *this;
}

// Operator[] - array-style access (non-const version)
template <typename T>
T& LinkedList<T>::operator[](int index)
{
    return at(index);  // Reuse at() which has bounds checking
}

// Operator[] - array-style access (const version)
template <typename T>
const T& LinkedList<T>::operator[](int index) const
{
    return at(index);
}

// ============================================
// HELPER FUNCTION
// ============================================

// copyList - deep copy helper function
// Copies all nodes from 'other' list into this list
template <typename T>
void LinkedList<T>::copyList(const LinkedList<T>& other)
{
    if (other.first == nullptr)  // Other list is empty
    {
        first = nullptr;
        last = nullptr;
        count = 0;
        return;
    }
    
    // Copy first node
    Node<T>* current = other.first;
    first = new Node<T>(current->data);
    last = first;
    count = 1;
    
    // Copy remaining nodes
    current = current->next;
    while (current != nullptr)
    {
        Node<T>* newNode = new Node<T>(current->data);
        last->next = newNode;  // Link previous node to new node
        last = newNode;        // Update last pointer
        count++;
        current = current->next;
    }
}

// ============================================
// BASIC OPERATIONS
// ============================================

// empty() - check if list has no elements
template <typename T>
bool LinkedList<T>::empty() const
{
    return first == nullptr;
}

// size() - return number of elements
template <typename T>
int LinkedList<T>::size() const
{
    return count;
}

// insertFront() - insert new node at beginning of list
// Example: list is [20, 30] -> insertFront(10) -> list becomes [10, 20, 30]
template <typename T>
void LinkedList<T>::insertFront(const T& item)
{
    Node<T>* newNode = new Node<T>(item, first);  // Create node pointing to current first
    
    if (first == nullptr)  // List was empty
    {
        first = newNode;
        last = newNode;
    }
    else  // List has elements
    {
        first = newNode;  // New node becomes first
    }
    
    count++;
}

// insertBack() - insert new node at end of list
// Example: list is [10, 20] -> insertBack(30) -> list becomes [10, 20, 30]
template <typename T>
void LinkedList<T>::insertBack(const T& item)
{
    Node<T>* newNode = new Node<T>(item);  // Create node with next = nullptr
    
    if (first == nullptr)  // List was empty
    {
        first = newNode;
        last = newNode;
    }
    else  // List has elements
    {
        last->next = newNode;  // Old last node points to new node
        last = newNode;        // Update last pointer
    }
    
    count++;
}

// push_back() - alias for insertBack (matches vector interface)
template <typename T>
void LinkedList<T>::push_back(const T& item)
{
    insertBack(item);
}

// deleteNode() - removes first occurrence of item from list
// Returns true if item was found and deleted, false otherwise
template <typename T>
bool LinkedList<T>::deleteNode(const T& item)
{
    if (first == nullptr)  // Empty list
    {
        return false;
    }
    
    Node<T>* current = first;
    Node<T>* previous = nullptr;
    
    // Search for node containing item
    while (current != nullptr && !(current->data == item))
    {
        previous = current;
        current = current->next;
    }
    
    if (current == nullptr)  // Item not found
    {
        return false;
    }
    
    // Item found - now delete the node
    if (previous == nullptr)  // Deleting first node
    {
        first = first->next;
        
        if (first == nullptr)  // List is now empty
        {
            last = nullptr;
        }
    }
    else  // Deleting middle or last node
    {
        previous->next = current->next;
        
        if (current == last)  // Deleting last node
        {
            last = previous;
        }
    }
    
    delete current;  // Free memory
    count--;
    return true;
}

// search() - returns true if item exists in list
template <typename T>
bool LinkedList<T>::search(const T& item) const
{
    Node<T>* current = first;
    
    while (current != nullptr)
    {
        if (current->data == item)
        {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

// clear() - delete all nodes from list
template <typename T>
void LinkedList<T>::clear()
{
    Node<T>* current = first;
    
    while (current != nullptr)
    {
        Node<T>* temp = current;      // Save current node
        current = current->next;       // Move to next
        delete temp;                   // Delete saved node
    }
    
    first = nullptr;
    last = nullptr;
    count = 0;
}

// ============================================
// ITERATOR SUPPORT
// ============================================

// begin() - return iterator to first element
template <typename T>
LinkedListIterator<T> LinkedList<T>::begin() const
{
    return LinkedListIterator<T>(first);
}

// end() - return iterator representing "past the end"
template <typename T>
LinkedListIterator<T> LinkedList<T>::end() const
{
    return LinkedListIterator<T>(nullptr);
}

// ============================================
// ARRAY-LIKE ACCESS
// ============================================

// at() - access element at index with bounds checking (non-const)
// Throws out_of_range exception if index is invalid
template <typename T>
T& LinkedList<T>::at(int index)
{
    if (index < 0 || index >= count)
    {
        throw out_of_range("LinkedList::at() - index out of range");
    }
    
    Node<T>* current = first;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    
    return current->data;
}

// at() - const version
template <typename T>
const T& LinkedList<T>::at(int index) const
{
    if (index < 0 || index >= count)
    {
        throw out_of_range("LinkedList::at() - index out of range");
    }
    
    Node<T>* current = first;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    
    return current->data;
}

#endif // LINKEDLIST_CPP