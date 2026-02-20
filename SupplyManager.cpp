#include "SupplyManager.h"
#include "GeneralSupplies.h"
#include "HikingSupplies.h"

// Default constructor
SupplyManager::SupplyManager()
	: items()  // WEEK 06: DynamicArray default constructor (capacity 5)
{
}

// Parameterized constructor
SupplyManager::SupplyManager(int initialCapacity)
	: items(initialCapacity)  // WEEK 06: DynamicArray parameterized constructor
{
}

// Destructor - CRITICAL for memory management
SupplyManager::~SupplyManager()
{
	// Delete all managed objects (SupplyList pointers)
	for (int i = 0; i < items.getSize(); i++)
	{
		delete items[i];  // Virtual destructor ensures proper cleanup
		// No need to set to nullptr - DynamicArray destructor handles array cleanup
	}
	// DynamicArray destructor automatically cleans up the array itself
}

// Copy constructor (deep copy)
SupplyManager::SupplyManager(const SupplyManager& other)
	: items(other.items.getCapacity())  // Create with same capacity
{
	// Note: Polymorphic copy is complex - would need clone() method
	// For now, we copy pointers (shallow copy of pointers themselves)
	// In production, each SupplyList would need a clone() virtual method
	for (int i = 0; i < other.items.getSize(); i++)
	{
		// WARNING: This is a shallow copy of pointers
		// Real implementation would need: items += other.items[i]->clone();
		items += other.items[i];
	}
}

// Add supply to manager (takes ownership)
void SupplyManager::addSupply(SupplyList* supply)
{
	if (supply == nullptr)
	{
		return;  // Don't add null pointers
	}
	
	// WEEK 06: Use DynamicArray - automatically resizes if needed
	items += supply;  // Uses operator+= from DynamicArray
	
	cout << "# Added supply: " << supply->getSupplyType() << " #" << endl;
}

// Remove supply at index (deletes the object)
bool SupplyManager::removeSupply(int index)
{
	if (index < 0 || index >= items.getSize())
	{
		return false;  // Invalid index
	}
	
	// Delete the object at index
	delete items[index];
	
	// WEEK 06: Use DynamicArray operator-= to remove and shift
	items -= index;  // Automatically shifts remaining elements
	
	return true;
}



// Remove all items
void SupplyManager::removeAll()
{
	for (int i = 0; i < items.getSize(); i++)
	{
		delete items[i];
	}
	items.clear();  // WEEK 06: Clear the DynamicArray
}

// Get supply at index (does not transfer ownership)
SupplyList* SupplyManager::getSupply(int index) const
{
	if (index < 0 || index >= items.getSize())
	{
		return nullptr;
	}
	return items[index];  // WEEK 06: Direct access via DynamicArray
}

// Get current size
int SupplyManager::getSize() const
{
	return items.getSize();  // WEEK 06: Delegate to DynamicArray
}

// Get capacity
int SupplyManager::getCapacity() const
{
	return items.getCapacity();  // WEEK 06: Delegate to DynamicArray
}

// WEEK 06 ADDITION: Operator[] for array-style access with bounds checking
SupplyList* SupplyManager::operator[](int index) const
{
	// Use 'this' pointer explicitly to demonstrate requirement
	if (index < 0 || index >= this->items.getSize())
	{
		cout << "# SupplyManager: Index " << index << " out of bounds. Returning nullptr. #" << endl;
		return nullptr;  // Safe return for out of bounds
	}
	return this->items[index];
}

// WEEK 06 ADDITION: Operator+= to add supply
SupplyManager& SupplyManager::operator+=(SupplyList* supply)
{
	if (supply != nullptr)
	{
		// Use 'this' pointer explicitly
		this->items += supply;  // DynamicArray handles resizing
		cout << "# Operator+=: Added supply: " << supply->getSupplyType() << " #" << endl;
	}
	return *this;  // Return reference for chaining
}

// WEEK 06 ADDITION: Operator-= to remove supply by index
SupplyManager& SupplyManager::operator-=(int index)
{
	// Use 'this' pointer explicitly
	if (index >= 0 && index < this->items.getSize())
	{
		// Delete the object first
		delete this->items[index];
		
		// Remove from array (shifts elements)
		this->items -= index;
		
		cout << "# Operator-=: Removed supply at index " << index << " #" << endl;
	}
	else
	{
		cout << "# Operator-=: Index " << index << " out of bounds. No removal. #" << endl;
	}
	return *this;  // Return reference for chaining
}

// Print all supplies polymorphically
void SupplyManager::printAll() const
{
	cout << "#*********************************#" << endl;
	cout << "#     Supply Manager Contents     #" << endl;
	cout << "#*********************************#" << endl;
	cout << "Total Supplies: " << items.getSize() << " / " << items.getCapacity() << endl << endl;
	
	if (items.getSize() == 0)
	{
		cout << "No supplies currently managed." << endl;
	}
	else
	{
		for (int i = 0; i < items.getSize(); i++)
		{
			cout << "[" << i << "] " << items[i]->getSupplyType() << endl;
			cout << "    Name: " << items[i]->getListName() << endl;
			cout << "    Priority: " << items[i]->getPriorityString(items[i]->getListPriority()) << endl;
			cout << "    Food Items: " << items[i]->getFoodCount() << endl;
			cout << "    Gear Items: " << items[i]->getGearCount() << endl;
			cout << endl;
		}
	}
	
	cout << "#*********************************#" << endl;
}

// Print summary
void SupplyManager::printSummary() const
{
	int totalFood = 0;
	int totalGear = 0;
	
	for (int i = 0; i < items.getSize(); i++)
	{
		totalFood += items[i]->getFoodCount();
		totalGear += items[i]->getGearCount();
	}
	
	cout << "#*********************************#" << endl;
	cout << "#   Supply Manager Summary        #" << endl;
	cout << "#*********************************#" << endl;
	cout << "Total Supply Lists: " << items.getSize() << endl;
	cout << "Total Food Items: " << totalFood << endl;
	cout << "Total Gear Items: " << totalGear << endl;
	cout << "#*********************************#" << endl;
}