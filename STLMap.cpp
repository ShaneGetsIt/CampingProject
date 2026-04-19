//***********************************************************
// Author: D.S. Malik Style
// This class specifies the basic operations on an STL map
// for camping supply items using the item name as the key
// and the item quantity as the value.
//***********************************************************

#include <iostream>
#include "STLMap.h"

using namespace std;

// Default constructor
STLMap::STLMap()
{
    initializeMap();
}

// Destructor
STLMap::~STLMap()
{
    initializeMap();
}

// Function to initialize the map to an empty state.
void STLMap::initializeMap()
{
    itemMap.clear();
}

// Function to determine whether the map is empty.
bool STLMap::isEmptyMap() const
{
    return itemMap.empty();
}

// Function to add itemName and quantity to the map.
void STLMap::insertItem(const string& itemName, int quantity)
{
    itemMap[itemName] = quantity;
}

// Function to retrieve the quantity for itemName.
bool STLMap::lookupItem(const string& itemName, int& quantity) const
{
    map<string, int>::const_iterator it;

    it = itemMap.find(itemName);

    if (it != itemMap.end())
    {
        quantity = it->second;
        return true;
    }

    return false;
}

// Function to remove itemName from the map.
bool STLMap::deleteItem(const string& itemName)
{
    return (itemMap.erase(itemName) > 0);
}

// Function to iterate through the map and print all key-value pairs.
void STLMap::printMap() const
{
    map<string, int>::const_iterator it;

    for (it = itemMap.begin(); it != itemMap.end(); ++it)
    {
        cout << it->first << ": " << it->second << endl;
    }
}

// Function to return the number of key-value pairs currently stored in the map.
int STLMap::getSize() const
{
    return static_cast<int>(itemMap.size());
}