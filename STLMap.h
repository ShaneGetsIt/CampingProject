//***********************************************************
// Author: D.S. Malik Style
// This class specifies the basic operations on an STL map
// for camping supply items using the item name as the key
// and the item quantity as the value.
//***********************************************************

#ifndef STLMAP_H
#define STLMAP_H

#include <iostream>
#include <map>
#include <string>

class STLMap
{
public:
    void initializeMap();
    // Function to initialize the map to an empty state.
    // Postcondition: itemMap is empty.

    bool isEmptyMap() const;
    // Function to determine whether the map is empty.
    // Postcondition: Returns true if the map is empty,
    //               otherwise returns false.

    void insertItem(const std::string& itemName, int quantity);
    // Function to add itemName and quantity to the map.
    // Precondition: The map exists.
    // Postcondition: The map is changed and the item is
    //               inserted. If the key already exists,
    //               the quantity is updated.

    bool lookupItem(const std::string& itemName, int& quantity) const;
    // Function to retrieve the quantity for itemName.
    // Precondition: The map exists.
    // Postcondition: If itemName exists, returns true and
    //               stores its quantity in quantity.
    //               Otherwise returns false.

    bool deleteItem(const std::string& itemName);
    // Function to remove itemName from the map.
    // Precondition: The map exists.
    // Postcondition: If itemName exists, it is removed
    //               and the function returns true.
    //               Otherwise returns false.

    void printMap() const;
    // Function to iterate through the map and print
    // all key-value pairs.

    int getSize() const;
    // Function to return the number of key-value pairs
    // currently stored in the map.

    STLMap();
    // Default constructor
    // Postcondition: itemMap is empty.

    ~STLMap();
    // Destructor
    // Postcondition: The map is cleared.

private:
    std::map<std::string, int> itemMap;
};

#endif

