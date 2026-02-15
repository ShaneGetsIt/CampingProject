#include "SupplyManager.h"
#include "GeneralSupplies.h"
#include "HikingSupplies.h"

// Default constructor
SupplyManager::SupplyManager()
{
	capacity = 5;  // Start with capacity of 5
	size = 0;
	items = new SupplyList*[capacity];  // Allocate array of pointers
}

// Parameterized constructor
SupplyManager::SupplyManager(int initialCapacity)
{
	capacity = (initialCapacity > 0) ? initialCapacity : 5;
	size = 0;
	items = new SupplyList*[capacity];
}

// Destructor - CRITICAL for memory management
SupplyManager::~SupplyManager()
{
	// Delete all managed objects
	for (int i = 0; i < size; i++)
	{
		delete items[i];  // Virtual destructor ensures proper cleanup
		items[i] = nullptr;
	}
	
	// Delete the array itself
	delete[] items;
	items = nullptr;
}

// Copy constructor (deep copy)
SupplyManager::SupplyManager(const SupplyManager& other)
{
	capacity = other.capacity;
	size = other.size;
	items = new SupplyList*[capacity];
	
	// Deep copy each item (Note: This is simplified - polymorphic copy is complex)
	for (int i = 0; i < size; i++)
	{
		// For now, we'll set to nullptr (full polymorphic copy requires clone() method)
		items[i] = nullptr;
	}
}

// Add supply to manager (takes ownership)
void SupplyManager::addSupply(SupplyList* supply)
{
	if (supply == nullptr)
	{
		return;  // Don't add null pointers
	}
	
	// Resize if needed
	if (size >= capacity)
	{
		resize();
	}
	
	// Add the item
	items[size] = supply;
	size++;
	
	cout << "# Added supply: " << supply->getSupplyType() << " #" << endl;
}

// Remove supply at index (deletes the object)
bool SupplyManager::removeSupply(int index)
{
	if (index < 0 || index >= size)
	{
		return false;  // Invalid index
	}
	
	// Delete the object at index
	delete items[index];
	
	// Shift remaining items down
	for (int i = index; i < size - 1; i++)
	{
		items[i] = items[i + 1];
	}
	
	size--;
	items[size] = nullptr;  // Clear the last slot
	
	return true;
}

// Remove all items
void SupplyManager::removeAll()
{
	for (int i = 0; i < size; i++)
	{
		delete items[i];
		items[i] = nullptr;
	}
	size = 0;
}

// Get supply at index (does not transfer ownership)
SupplyList* SupplyManager::getSupply(int index) const
{
	if (index < 0 || index >= size)
	{
		return nullptr;
	}
	return items[index];
}

// Get current size
int SupplyManager::getSize() const
{
	return size;
}

// Get capacity
int SupplyManager::getCapacity() const
{
	return capacity;
}

// Print all supplies polymorphically
void SupplyManager::printAll() const
{
	cout << "#*********************************#" << endl;
	cout << "#     Supply Manager Contents     #" << endl;
	cout << "#*********************************#" << endl;
	cout << "Total Supplies: " << size << " / " << capacity << endl << endl;
	
	if (size == 0)
	{
		cout << "No supplies currently managed." << endl;
	}
	else
	{
		for (int i = 0; i < size; i++)
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
	
	for (int i = 0; i < size; i++)
	{
		totalFood += items[i]->getFoodCount();
		totalGear += items[i]->getGearCount();
	}
	
	cout << "#*********************************#" << endl;
	cout << "#   Supply Manager Summary        #" << endl;
	cout << "#*********************************#" << endl;
	cout << "Total Supply Lists: " << size << endl;
	cout << "Total Food Items: " << totalFood << endl;
	cout << "Total Gear Items: " << totalGear << endl;
	cout << "#*********************************#" << endl;
}

// Resize array when capacity is reached
void SupplyManager::resize()
{
	int newCapacity = capacity * 2;  // Double the capacity
	SupplyList** newItems = new SupplyList*[newCapacity];
	
	// Copy existing pointers
	for (int i = 0; i < size; i++)
	{
		newItems[i] = items[i];
	}
	
	// Delete old array (but NOT the objects it points to)
	delete[] items;
	
	// Update to new array
	items = newItems;
	capacity = newCapacity;
	
	cout << "# Manager resized to capacity: " << capacity << " #" << endl;
}