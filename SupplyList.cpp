#include "SupplyList.h"

// Default constructor
SupplyList::SupplyList()
    : listName("General Supplies"), maxCapacity(MAX_ARRAY), listPriority(medium)
{
    // vectors default-construct empty
}

// Parameterized constructor
SupplyList::SupplyList(string name, int capacity, priority prio)
    : listName(name), maxCapacity(capacity), listPriority(prio)
{
    // vectors default-construct empty
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
                if (static_cast<int>(foodList.size()) < length)
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
                    foodList.push_back(temp);
                }
                else
                    cout << " Limit for additional food items reached" << endl << endl;
                break;
            }

            case gear:
            {
                // Add gear entry if room
                if (static_cast<int>(gearList.size()) < length)
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
                    gearList.push_back(temp);
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

    if (!foodList.empty())
    {
        outData << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra food to pack" << "Priority" << endl;

        for (size_t i = 0; i < foodList.size(); ++i)
        {
            const inventoryItem& it = foodList[i];
            outData << left << it.quantity << setw(30) << "x " + it.itemName;
            if (it.prio == 0)
            {
                outData << right << setw(4) << "Low" << endl;
            }
            else if (it.prio == 1)
            {
                outData << right << setw(4) << "Med" << endl;
            }
            else if (it.prio == 2)
            {
                outData << right << setw(4) << "High" << endl;
            }
        }
    }

    if (!gearList.empty())
    {
        outData << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra gear to pack" << "Priority" << endl;

        for (size_t i = 0; i < gearList.size(); ++i)
        {
            const inventoryItem& it = gearList[i];
            outData << left << it.quantity << setw(30) << "x " + it.itemName;
            if (it.prio == 0)
            {
                outData << right << setw(4) << "Low" << endl;
            }
            else if (it.prio == 1)
            {
                outData << right << setw(4) << "Med" << endl;
            }
            else if (it.prio == 2)
            {
                outData << right << setw(4) << "High" << endl;
            }
        }
    }
}

// Print stored extra items to console
void SupplyList::printItems(int length)
{
    if (!foodList.empty())
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra food to pack" << "Priority" << endl;

        for (size_t i = 0; i < foodList.size(); ++i)
        {
            const inventoryItem& it = foodList[i];
            cout << left << it.quantity << setw(30) << "x " + it.itemName;
            if (it.prio == 0)
            {
                cout << right << setw(4) << "Low" << endl;
            }
            else if (it.prio == 1)
            {
                cout << right << setw(4) << "Med" << endl;
            }
            else if (it.prio == 2)
            {
                cout << right << setw(4) << "High" << endl;
            }
        }
    }

    if (!gearList.empty())
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra gear to pack" << "Priority" << endl;

        for (size_t i = 0; i < gearList.size(); ++i)
        {
            const inventoryItem& it = gearList[i];
            cout << left << it.quantity << setw(30) << "x " + it.itemName;
            if (it.prio == 0)
            {
                cout << right << setw(4) << "Low" << endl;
            }
            else if (it.prio == 1)
            {
                cout << right << setw(4) << "Med" << endl;
            }
            else if (it.prio == 2)
            {
                cout << right << setw(4) << "High" << endl;
            }
        }
    }
}

// Test-friendly methods (non-interactive)

// Add food item without user interaction; returns false if list is full
bool SupplyList::addFoodItem(const inventoryItem& item)
{
    if (static_cast<int>(foodList.size()) >= MAX_ARRAY) return false;
    foodList.push_back(item);
    return true;
}

// Add gear item without user interaction; returns false if list is full
bool SupplyList::addGearItem(const inventoryItem& item)
{
    if (static_cast<int>(gearList.size()) >= MAX_ARRAY) return false;
    gearList.push_back(item);
    return true;
}

// Get current food count
int SupplyList::getFoodCount() const
{
    return static_cast<int>(foodList.size());
}

// Get current gear count
int SupplyList::getGearCount() const
{
    return static_cast<int>(gearList.size());
}

// Get total item count (food + gear)
int SupplyList::getTotalItemCount() const
{
    return getFoodCount() + getGearCount();
}

// Calculate average food quantity (guards against divide by zero)
double SupplyList::getAverageFoodQuantity() const
{
    if (foodList.empty()) return 0.0;

    int total = 0;
    for (size_t i = 0; i < foodList.size(); ++i)
    {
        total += foodList[i].quantity;
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
    return foodList.at(index);
}

// Get gear item at index (for testing)
const SupplyList::inventoryItem& SupplyList::getGearItem(int index) const
{
    return gearList.at(index);
}

// Clear all items (for testing)
void SupplyList::clearAll()
{
    foodList.clear();
    gearList.clear();
}

// Sequential (linear) search on foodList - returns index or -1 if not found
int SupplyList::sequentialSearchFood(const string& name) const

{        //using size_t for loop index to avoid signed/unsigned mismatch with vector size


    for (size_t i = 0; i < foodList.size(); ++i)
    {
        if (foodList[i].itemName == name)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Insertion sort (by itemName) for foodList
void SupplyList::insertionSortFoodByName()
{
    // Classic insertion sort operating on vector<inventoryItem>
    for (size_t i = 1; i < foodList.size(); ++i)
    {
        inventoryItem key = foodList[i];
        int j = static_cast<int>(i) - 1;

        // Compare by itemName
        while (j >= 0 && foodList[static_cast<size_t>(j)].itemName > key.itemName)
        {
            foodList[static_cast<size_t>(j + 1)] = foodList[static_cast<size_t>(j)];
            --j;
        }
        foodList[static_cast<size_t>(j + 1)] = key;
    }
}

// Binary search on foodList by itemName - requires sorted foodList (ascending)
// Classic low/mid/high implementation
int SupplyList::binarySearchFoodByName(const string& name) const
{
    if (foodList.empty()) return -1;

    int low = 0;
    int high = static_cast<int>(foodList.size()) - 1;

    while (low <= high)
    {
		int mid = low + (high - low) / 2; // Avoids potential overflow
        const string& midName = foodList[static_cast<size_t>(mid)].itemName;

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

// Virtual destructor (ADDED for abstract base class)
SupplyList::~SupplyList()
{
    // No dynamic memory to clean up in base class
    // Derived classes will handle their own cleanup
}

// WEEK 06 ADDITION: Operator<< overload as non-member friend function
// Uses polymorphism by calling virtual toStream() method
ostream& operator<<(ostream& out, const SupplyList& supply)
{
    // Call the virtual toStream() method - polymorphic behavior
    supply.toStream(out);
    return out;
}