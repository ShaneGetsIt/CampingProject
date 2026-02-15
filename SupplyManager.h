#pragma once

#include "SupplyList.h"
#include <iostream>
using namespace std;

// SupplyManager - manages a dynamic array of SupplyList pointers
class SupplyManager
{
private:
	SupplyList** items;     // Dynamic array of base class pointers
	int size;               // Current number of items
	int capacity;           // Maximum capacity

	// Helper function to resize array when full
	void resize();

public:
	// Constructors
	SupplyManager();
	SupplyManager(int initialCapacity);

	// Destructor - must clean up all dynamic memory
	~SupplyManager();

	// Copy constructor (prevent shallow copy issues)
	SupplyManager(const SupplyManager& other);

	// Core functionality
	void addSupply(SupplyList* supply);           // Add item (takes ownership)
	bool removeSupply(int index);                 // Remove and delete item at index
	void removeAll();                             // Remove and delete all items

	// Access
	SupplyList* getSupply(int index) const;       // Get pointer to item at index
	int getSize() const;                          // Get current size
	int getCapacity() const;                      // Get capacity

	// Polymorphic operations
	void printAll() const;                        // Print all items using virtual functions
	void printSummary() const;                    // Print summary of all supplies
};