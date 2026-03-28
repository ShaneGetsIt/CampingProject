#pragma once

#include <iostream>
#include <stdexcept>
using namespace std;

// Forward declarations
template <typename T>
class LinkedList;

template <typename T>
class LinkedListIterator;

// ============================================
// NODE STRUCTURE
// ============================================
// Basic building block of the linked list
// Contains: data of type T, pointer to next node
template <typename T>
struct Node
{
    T data;           // The actual data stored
    Node<T>* next;    // Pointer to the next node in the list
    
    // Constructor: initialize node with data and optional next pointer
    Node(const T& item, Node<T>* ptr = nullptr)
        : data(item), next(ptr)
    {
    }
};

// ============================================
// LINKED LIST ITERATOR
// ============================================
// Allows us to traverse the list like we would with vector iterators
// Example: for (auto iter = list.begin(); iter != list.end(); ++iter)
template <typename T>
class LinkedListIterator
{
private:
    Node<T>* current;  // Points to the current node in iteration
    
public:
    // Constructor: start iteration at given node
    LinkedListIterator(Node<T>* ptr = nullptr)
        : current(ptr)
    {
    }
    
    // Dereference operator: *iter returns the data
    T& operator*()
    {
        return current->data;
    }
    
    // Arrow operator: iter->member accesses data members
    T* operator->()
    {
        return &(current->data);
    }
    
    // Pre-increment: ++iter moves to next node
    LinkedListIterator<T>& operator++()
    {
        if (current != nullptr)
        {
            current = current->next;
        }
        return *this;
    }
    
    // Equality: check if two iterators point to same node
    bool operator==(const LinkedListIterator<T>& other) const
    {
        return current == other.current;
    }
    
    // Inequality: opposite of ==
    bool operator!=(const LinkedListIterator<T>& other) const
    {
        return current != other.current;
    }
};

// ============================================
// LINKED LIST CLASS (UNORDERED)
// ============================================
// Unordered linked list with insertFront and insertBack options
template <typename T>
class LinkedList
{
private:
    Node<T>* first;   // Pointer to first node (head)
    Node<T>* last;    // Pointer to last node (tail)
    int count;        // Number of nodes in list
    
    // Helper function for deep copy
    void copyList(const LinkedList<T>& other);
    
public:
    // ========== Constructors and Destructor ==========
    
    // Default constructor: creates empty list
    LinkedList();
    
    // Copy constructor: creates deep copy of another list
    LinkedList(const LinkedList<T>& other);
    
    // Destructor: deletes all nodes and frees memory
    ~LinkedList();
    
    // Overloaded assignment operator: deep copy
    const LinkedList<T>& operator=(const LinkedList<T>& other);
    
    // ========== Basic Operations ==========
    
    // Check if list is empty
    bool empty() const;
    
    // Get number of elements
    int size() const;
    
    // Insert at front of list (like push_front)
    void insertFront(const T& item);
    
    // Insert at back of list (like push_back)
    void insertBack(const T& item);
    
    // Alias for insertBack (matches vector interface)
    void push_back(const T& item);
    
    // Delete first occurrence of item from list
    bool deleteNode(const T& item);
    
    // Search for item in list
    bool search(const T& item) const;
    
    // Clear all nodes from list
    void clear();
    
    // ========== Iterator Support ==========
    
    // Get iterator to beginning
    LinkedListIterator<T> begin() const;
    
    // Get iterator to end (nullptr)
    LinkedListIterator<T> end() const;
    
    // ========== Array-like Access ==========
    
    // Access element at index (throws if out of range)
    T& at(int index);
    const T& at(int index) const;
    
    // Operator[] for array-style access
    T& operator[](int index);
    const T& operator[](int index) const;
};

// Include implementation (required for templates)
#include "LinkedList.cpp"