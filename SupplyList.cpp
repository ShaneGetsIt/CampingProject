#include "SupplyList.h"
#include "json.hpp"

using json = nlohmann::json;

// Default constructor
SupplyList::SupplyList()
    : listName("General Supplies"), maxCapacity(MAX_ARRAY), listPriority(medium)
{
    // WEEK 08: LinkedLists default-construct empty (changed from vectors)
}

// Parameterized constructor
SupplyList::SupplyList(string name, int capacity, priority prio)
    : listName(name), maxCapacity(capacity), listPriority(prio)
{
    // WEEK 08: LinkedLists default-construct empty (changed from vectors)
}

// Getters for base class data
string SupplyList::getListName() const
{
    return listName;
}

int SupplyList::getMaxCapacity() const
{
    return maxCapacity;
}

priority SupplyList::getListPriority() const
{
    return listPriority;
}

// Setters for base class data
void SupplyList::setListName(string name)
{
    listName = name;
}

void SupplyList::setMaxCapacity(int capacity)
{
    maxCapacity = capacity;
}

void SupplyList::setListPriority(priority prio)
{
    listPriority = prio;
}

// Prompt user to add extra food/gear items and store them in the lists
void SupplyList::extrasFunc(char ch, int length)
{
    int choice = 0;

    if (ch == 'y' || ch == 'Y') {

        cin.ignore(1, '\n');
        cout << "# You may add up to " << length << " food items and/or #" << endl
            << "#===== - - - -            " << length << " gear items =#" << endl << endl;

        while (true)
        {
            cout << endl << "#== Please enter '1' to add food, '2' ==#" << endl
                << "#=====  to add gear or '3' to stop  ====#" << endl << endl;
            cin >> choice;

            while (!choice || choice > 3 || choice < 1)
            {
                cout << "Invalid selection - must enter int's 1-3" << endl;
                cin.clear();
                cin.ignore(12, '\n');
                cin >> choice;
            }

            switch (static_cast<itemType>(choice - 1))
            {
            case food:
            {
                // Add food entry if room
                if (foodList.size() < length)  // WEEK 08: LinkedList::size()
                {
                    inventoryItem temp;
                    temp.item = food;

                    cin.ignore(50, '\n');
                    cout << "#= Enter food item name to add to list =#" << endl << endl;
                    getline(cin, temp.itemName);
                    cout << "# Units of " << left << setfill('.') << setw(30) << temp.itemName
                        + " to bring " << "#" << endl;
                    cin >> temp.quantity;
                    cout << "# How important is this item on a scale #" << endl
                        << "#= of 1 (low), 2 (medium), or 3 (high)? #" << endl << endl;
                    cin >> choice;
                    while (!choice || choice < 1 || choice > 3)
                    {
                        cout << " Invalid selection - must enter int's 1-3" << endl;
                        cin.clear();
                        cin.ignore(12, '\n');
                        cin >> choice;
                    }
                    temp.prio = static_cast<priority>(choice - 1);
                    foodList.push_back(temp);  // WEEK 08: LinkedList::push_back()
                }
                else
                    cout << " Limit for additional food items reached" << endl << endl;
                break;
            }

            case gear:
            {
                // Add gear entry if room
                if (gearList.size() < length)  // WEEK 08: LinkedList::size()
                {
                    inventoryItem temp;
                    temp.item = gear;
                    cin.ignore(50, '\n');
                    cout << "#= Enter gear item name to add to list =#" << endl
                        << "# for example: kayak, fishing rod, etc. #" << endl << endl;
                    getline(cin, temp.itemName);
                    cout << "# Units of " << left << setfill('.') << setw(30) << temp.itemName
                        + " to bring " << "#" << endl;
                    cin >> temp.quantity;
                    cout << "# How important is this item on a scale #" << endl
                        << "#= of 1 (low), 2 (medium), or 3 (high)? #" << endl << endl;
                    cin >> choice;
                    while (!choice || choice < 1 || choice > 3)
                    {
                        cout << " Invalid selection - must enter int's 1-3" << endl;
                        cin.clear();
                        cin.ignore(12, '\n');
                        cin >> choice;
                    }
                    temp.prio = static_cast<priority>(choice - 1);
                    gearList.push_back(temp);  // WEEK 08: LinkedList::push_back()
                }
                else
                    cout << " Limit for additional gear items reached" << endl << endl;
                break;
            }

            default:
                cout << "#== Finished adding additional items  ==#" << endl << endl;
                return;
            }
        }
    }
    else
        cout << "#===   No extra items to add - got it! =#" << endl << endl;
}

// Write stored extra items to the provided output stream
void SupplyList::addItems(ofstream& outData, bool& raiseFlag, int length)
{
    if (!outData)
    {
        cout << "#=====   Output file is not open   =====#" << endl << endl;
        raiseFlag = true;
        return;
    }

    if (!foodList.empty())  // WEEK 08: LinkedList::empty()
    {
        outData << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra food to pack" << "Priority" << endl;

        // WEEK 08: Can still use index-based loop with LinkedList::at()
        for (int i = 0; i < foodList.size(); ++i)
        {
            const inventoryItem& it = foodList.at(i);  // WEEK 08: LinkedList::at()
            outData << left << it.quantity << setw(30) << "x " + it.itemName;
            outData << right << setw(4) << getPriorityString(it.prio) << endl;
        }
    }

    if (!gearList.empty())  // WEEK 08: LinkedList::empty()
    {
        outData << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra gear to pack" << "Priority" << endl;

        // WEEK 08: Can still use index-based loop
        for (int i = 0; i < gearList.size(); ++i)
        {
            const inventoryItem& it = gearList.at(i);  // WEEK 08: LinkedList::at()
            outData << left << it.quantity << setw(30) << "x " + it.itemName;
            outData << right << setw(4) << getPriorityString(it.prio) << endl;
        }
    }
}

// Print stored extra items to console
void SupplyList::printItems(int length)
{
    if (!foodList.empty())  // WEEK 08: LinkedList::empty()
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra food to pack" << "Priority" << endl;

        // WEEK 08: Using iterator - demonstrates LinkedListIterator usage
        LinkedListIterator<inventoryItem> iter = foodList.begin();
        while (iter != foodList.end())
        {
            cout << left << iter->quantity << setw(30) << "x " + iter->itemName;
            cout << right << setw(4) << getPriorityString(iter->prio) << endl;
            ++iter;
        }
    }

    if (!gearList.empty())  // WEEK 08: LinkedList::empty()
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra gear to pack" << "Priority" << endl;

        // WEEK 08: Using iterator for gear list too
        LinkedListIterator<inventoryItem> iter = gearList.begin();
        while (iter != gearList.end())
        {
            cout << left << iter->quantity << setw(30) << "x " + iter->itemName;
            cout << right << setw(4) << getPriorityString(iter->prio) << endl;
            ++iter;
        }
    }
}

// Test-friendly methods (non-interactive)

// Add food item without user interaction; returns false if list is full
bool SupplyList::addFoodItem(const inventoryItem& item)
{
    if (foodList.size() >= MAX_ARRAY) return false;  // WEEK 08: LinkedList::size()
    foodList.push_back(item);  // WEEK 08: LinkedList::push_back()
    return true;
}

// Add gear item without user interaction; returns false if list is full
bool SupplyList::addGearItem(const inventoryItem& item)
{
    if (gearList.size() >= MAX_ARRAY) return false;  // WEEK 08: LinkedList::size()
    gearList.push_back(item);  // WEEK 08: LinkedList::push_back()
    return true;
}

// Get current food count
int SupplyList::getFoodCount() const
{
    return foodList.size();  // WEEK 08: LinkedList::size()
}

// Get current gear count
int SupplyList::getGearCount() const
{
    return gearList.size();  // WEEK 08: LinkedList::size()
}

// Get total item count (food + gear)
int SupplyList::getTotalItemCount() const
{
    return getFoodCount() + getGearCount();
}

// Calculate average food quantity (guards against divide by zero)
double SupplyList::getAverageFoodQuantity() const
{
    if (foodList.empty()) return 0.0;  // WEEK 08: LinkedList::empty()

    int total = 0;
    
    // WEEK 08: Using iterator to demonstrate LinkedList traversal
    LinkedListIterator<inventoryItem> iter = foodList.begin();
    while (iter != foodList.end())
    {
        total += iter->quantity;
        ++iter;
    }
    
    return static_cast<double>(total) / static_cast<double>(foodList.size());
}

// Convert priority enum to string using switch
string SupplyList::getPriorityString(priority prio) const
{
    switch (prio)
    {
    case low:
        return "Low";
    case medium:
        return "Med";
    case high:
        return "High";
    default:
        return "Unknown";
    }
}

// Get food item at index (for testing)
const SupplyList::inventoryItem& SupplyList::getFoodItem(int index) const
{
    return foodList.at(index);  // WEEK 08: LinkedList::at()
}

// Get gear item at index (for testing)
const SupplyList::inventoryItem& SupplyList::getGearItem(int index) const
{
    return gearList.at(index);  // WEEK 08: LinkedList::at()
}

// Clear all items (for testing)
void SupplyList::clearAll()
{
    foodList.clear();  // WEEK 08: LinkedList::clear()
    gearList.clear();  // WEEK 08: LinkedList::clear()
}

int SupplyList::loadItemsFromJsonFile(const string& filename)
{
    try
    {
        ifstream inFile(filename);

        if (!inFile)
        {
            throw runtime_error("JSON file could not be opened: " + filename);
        }

        json data;
        inFile >> data;

        if (!data.is_array())
        {
            throw runtime_error("JSON root must be an array.");
        }

        int loadedCount = 0;

        // Week 13 JSON: each JSON object becomes an inventoryItem and is added
        // to the existing LinkedList-backed foodList or gearList.
        for (const auto& entry : data)
        {
            string itemName = entry.at("itemName").get<string>();
            string category = entry.at("category").get<string>();
            string priorityText = entry.at("priority").get<string>();
            int quantity = entry.at("quantity").get<int>();

            inventoryItem item;
            item.itemName = itemName;
            item.quantity = quantity;

            if (priorityText == "low")
            {
                item.prio = low;
            }
            else if (priorityText == "medium")
            {
                item.prio = medium;
            }
            else if (priorityText == "high")
            {
                item.prio = high;
            }
            else
            {
                throw runtime_error("Invalid priority in JSON: " + priorityText);
            }

            if (category == "food")
            {
                item.item = food;
                if (addFoodItem(item))
                {
                    loadedCount++;
                }
            }
            else if (category == "gear")
            {
                item.item = gear;
                if (addGearItem(item))
                {
                    loadedCount++;
                }
            }
            else
            {
                throw runtime_error("Invalid category in JSON: " + category);
            }
        }

        return loadedCount;
    }
    catch (const exception& ex)
    {
        cout << "# JSON load error: " << ex.what() << " #" << endl;
        return 0;
    }
}

// Sequential (linear) search on foodList - returns index or -1 if not found
int SupplyList::sequentialSearchFood(const string& name) const
{
    // WEEK 08: Using iterator to traverse LinkedList
    LinkedListIterator<inventoryItem> iter = foodList.begin();
    int index = 0;
    
    while (iter != foodList.end())
    {
        if (iter->itemName == name)
        {
            return index;
        }
        ++iter;
        index++;
    }
    
    return -1;
}

// Insertion sort (by itemName) for foodList
void SupplyList::insertionSortFoodByName()
{
    // WEEK 08: Insertion sort works with LinkedList using at() for access
    // Note: This is O(n²) but demonstrates LinkedList can support sorting
    for (int i = 1; i < foodList.size(); ++i)
    {
        inventoryItem key = foodList.at(i);
        int j = i - 1;

        // Compare by itemName
        while (j >= 0 && foodList.at(j).itemName > key.itemName)
        {
            // Note: This creates a copy - LinkedList at() returns reference
            inventoryItem temp = foodList.at(j);
            foodList.at(j + 1) = temp;
            --j;
        }
        foodList.at(j + 1) = key;
    }
}

// Binary search on foodList by itemName - requires sorted foodList (ascending)
// Classic low/mid/high implementation
int SupplyList::binarySearchFoodByName(const string& name) const
{
    if (foodList.empty()) return -1;  // WEEK 08: LinkedList::empty()

    int low = 0;
    int high = foodList.size() - 1;  // WEEK 08: LinkedList::size()

    while (low <= high)
    {
        int mid = low + (high - low) / 2; // Avoids potential overflow
        const string& midName = foodList.at(mid).itemName;  // WEEK 08: LinkedList::at()

        if (midName == name)
        {
            return mid;
        }
        else if (midName < name)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1; // not found
}

// Virtual method - returns supply type (base class version)
// Derived classes (GeneralSupplies, HikingSupplies) override this
string SupplyList::getSupplyType() const
{
    return "Generic Supply List";
}

// Virtual method - outputs supply info to stream
// Derived classes override this for polymorphic output
void SupplyList::toStream(ostream& out) const
{
    out << "Supply List: " << listName << endl;
    out << "Capacity: " << maxCapacity << endl;
    out << "Priority: " << getPriorityString(listPriority) << endl;
    out << "Total Items: " << getTotalItemCount() << endl;
    out << "Food Items: " << getFoodCount() << endl;
    out << "Gear Items: " << getGearCount() << endl;
}

// WEEK 06 ADDITION: Operator== for comparing two SupplyList objects
bool SupplyList::operator==(const SupplyList& other) const
{
    return (listName == other.listName && 
            maxCapacity == other.maxCapacity && 
            listPriority == other.listPriority);
}

// Virtual destructor
SupplyList::~SupplyList()
{
    // WEEK 08: LinkedList destructor automatically cleans up nodes
    // No manual cleanup needed
}

// WEEK 06 ADDITION: Operator<< overload as non-member friend function
// Uses polymorphism by calling virtual toStream() method
ostream& operator<<(ostream& out, const SupplyList& supply)
{
    // Call the virtual toStream() method - polymorphic behavior
    supply.toStream(out);
    return out;
}