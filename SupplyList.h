#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include "LinkedList.h"  // WEEK 08: Replace <vector> with custom LinkedList

using namespace std;

const int MAX_ARRAY = 10;

enum priority { low, medium, high };

class SupplyList
{
public:
    enum itemType { food, gear, other };

    // WEEK 08: Add equality operator for LinkedList operations
    struct inventoryItem
    {
        string itemName;
        itemType item;
        priority prio;
        int quantity;
        
        // Equality operator - needed for LinkedList deleteNode() and search()
        bool operator==(const inventoryItem& other) const
        {
            // Two items are equal if they have the same name and type
            return (itemName == other.itemName && item == other.item);
        }
    };

private:
    string listName;
    int maxCapacity;
    priority listPriority;
    
    // WEEK 08: Replace vector with LinkedList
    LinkedList<inventoryItem> foodList;
    LinkedList<inventoryItem> gearList;

public:
    // Constructors
    SupplyList();
    SupplyList(string name, int capacity, priority prio);
    virtual ~SupplyList();

    // Getters/Setters
    string getListName() const;
    int getMaxCapacity() const;
    priority getListPriority() const;
    void setListName(string name);
    void setMaxCapacity(int capacity);
    void setListPriority(priority prio);

    // Interactive functions
    void extrasFunc(char ch, int length);
    void addItems(ofstream& outData, bool& raiseFlag, int length);
    void printItems(int length);

    // Test-friendly methods
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
    int loadItemsFromJsonFile(const string& filename);

    // Search/Sort operations
    int sequentialSearchFood(const string& name) const;
    void insertionSortFoodByName();
    int binarySearchFoodByName(const string& name) const;

    // Virtual methods for polymorphism
    virtual string getSupplyType() const;
    virtual void toStream(ostream& out) const;

    // Operator overloading
    friend ostream& operator<<(ostream& out, const SupplyList& supply);
    bool operator==(const SupplyList& other) const;
};