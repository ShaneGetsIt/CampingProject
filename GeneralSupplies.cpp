#include "GeneralSupplies.h"

// Default constructor - delegates to base class
GeneralSupplies::GeneralSupplies()
    : SupplyList()
{
}

// Parameterized constructor - delegates to base class
GeneralSupplies::GeneralSupplies(string name, int capacity, priority prio)
    : SupplyList(name, capacity, prio)
{
}

// Implement pure virtual function
string GeneralSupplies::getSupplyType() const
{
    return "General Camping Supplies";
}

// Virtual destructor
GeneralSupplies::~GeneralSupplies()
{
    // No additional cleanup needed
}


// Copy from base class (ADDED)
void GeneralSupplies::copyFrom(const SupplyList& other)
{
    // Copy base class data
    setListName(other.getListName());
    setMaxCapacity(other.getMaxCapacity());
    setListPriority(other.getListPriority());
    
    // Clear existing items
    clearAll();
    
    // Copy food items
    for (int i = 0; i < other.getFoodCount(); i++)
    {
        addFoodItem(other.getFoodItem(i));
    }
    
    // Copy gear items
    for (int i = 0; i < other.getGearCount(); i++)
    {
        addGearItem(other.getGearItem(i));
    }
}