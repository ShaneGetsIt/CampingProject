#pragma once

#include "SupplyList.h"
#include "DynamicArray.h"
#include <iostream>
using namespace std;

// WEEK 06 MODIFIED: SupplyManager now uses DynamicArray template
// Manages a dynamic array of SupplyList pointers
class SupplyManager
{
private:
	DynamicArray<SupplyList*> items;  // WEEK 06: Template replaces raw pointer array

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

	// WEEK 06 ADDITION: Operator overloads
	SupplyList* operator[](int index) const;      // Array access with bounds checking
	SupplyManager& operator+=(SupplyList* supply); // Add supply using operator
	SupplyManager& operator-=(int index);         // Remove supply by index using operator

	// Polymorphic operations
	void printAll() const;                        // Print all items using virtual functions
	void printSummary() const;                    // Print summary of all supplies
};