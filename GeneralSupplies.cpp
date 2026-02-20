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

// WEEK 06 ADDITION: Operator== overload
// Two GeneralSupplies objects are equal if their meaningful identity fields match
// Meaningful identity: listName, maxCapacity, listPriority
// Implemented as member function since we need access to private members via getters
bool GeneralSupplies::operator==(const GeneralSupplies& other) const
{
    // Compare meaningful identity fields using 'this' pointer explicitly
    return (this->getListName() == other.getListName() &&
            this->getMaxCapacity() == other.getMaxCapacity() &&
            this->getListPriority() == other.getListPriority());
}

// WEEK 06 ADDITION: Virtual function for polymorphic streaming
// Called by operator<< in base class
void GeneralSupplies::toStream(ostream& out) const
{
    // Use 'this' pointer explicitly to demonstrate requirement
    out << "GeneralSupplies[" << this->getListName() 
        << ", Priority: " << this->getPriorityString(this->getListPriority())
        << ", Items: " << this->getTotalItemCount() << "]";
}