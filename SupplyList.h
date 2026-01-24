#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

// Max extra items per category
const int MAX_ARRAY = 5;

// Types for items and priority
enum itemType { food, gear };
enum priority { low, medium, high };

// Simple POD to hold an inventory item
struct inventoryItem
{
    string itemName;
    itemType item = food;
    priority prio = low;
    int quantity = 0;
};

// SupplyList encapsulates additional food/gear items entered by user
class SupplyList
{
public:
    SupplyList() {}// default constructor

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

private:
    inventoryItem foodList[MAX_ARRAY]; // stored extra food items
    inventoryItem gearList[MAX_ARRAY]; // stored extra gear items
    int foodCount = 0;                 // number of food entries
    int gearCount = 0;                 // number of gear entries
};