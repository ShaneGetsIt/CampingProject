#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

// Max extra items per category
const int MAX_ARRAY = 5;

// Global priority enum (used across multiple base classes)
enum priority { low, medium, high };

// SupplyList encapsulates additional food/gear items entered by user
class SupplyList
{
public:
    // Nested types (moved inside class)
    enum itemType { food, gear };
    
    struct inventoryItem
    {
        string itemName;
        itemType item = food;
        priority prio = low;
        int quantity = 0;
    };

    // Default constructor
    SupplyList();
    
    // Parameterized constructor (makes this a proper base class)
    SupplyList(string name, int capacity, priority prio);

    // Collect extras from user (interactive)
    void extrasFunc(char ch, int length);
    // Write extra items to output file
    void addItems(ofstream& outData, bool& raiseFlag, int length);
    // Print extra items to console
    void printItems(int length);

    // Test-friendly methods (non-interactive)
    bool addFoodItem(const inventoryItem& item);
    bool addGearItem(const inventoryItem& item);
    int getFoodCount() const;
    int getGearCount() const;
    int getTotalItemCount() const;
    double getAverageFoodQuantity() const;
    string getPriorityString(priority prio) const;
    const inventoryItem& getFoodItem(int index) const;
    const inventoryItem& getGearItem(int index) const;
    void clearAll();
    
    // Getters for base class data
    string getListName() const;
    int getMaxCapacity() const;
    priority getListPriority() const;
    
    // Setters for base class data
    void setListName(string name);
    void setMaxCapacity(int capacity);
    void setListPriority(priority prio);

protected:
    // Base class data members (required by assignment)
    string listName;           // Name of this supply list
    int maxCapacity;           // Maximum items
    priority listPriority;     // Overall priority of this supply category

private:
    inventoryItem foodList[MAX_ARRAY]; // stored extra food items
    inventoryItem gearList[MAX_ARRAY]; // stored extra gear items
    int foodCount = 0;                 // number of food entries
    int gearCount = 0;                 // number of gear entries
};