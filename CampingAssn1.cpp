// Main program: gathers trip info, uses SupplyList to collect extras, and prints/saves report
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "SupplyList.h"
#include "Itin.h"
#include "HikingSupplies.h"
#include "GeneralSupplies.h"
#include "Menu.h"
#include "Logs.h"
#include "LogSelect.h"
#include "SupplyManager.h"
#include "DynamicArray.h"  // WEEK 06 ADDITION
#include "LinkedList.h"    // WEEK 08 ADDITION
#include "DebugMemoryCheck.h" // Added: CRT memory leak checker (Debug-only)

const string extras = "Extras.txt"; // legacy filenames
const string it = "It.txt";         // itinerary file
const string rpt = "Report.txt";    // report output file
const int COLOR = 100;              // console color attribute

// WEEK 06 ADDITION: Function template - returns the maximum of two values
// Works with any type T that supports operator>
template <typename T>
T maxValue(const T& a, const T& b)
{
	return (a > b) ? a : b;
}

// WEEK 06 ADDITION: Function template - clamps a value between min and max
// Works with any type T that supports comparison operators
template <typename T>
T clamp(const T& value, const T& minVal, const T& maxVal)
{
	if (value < minVal) return minVal;
	if (value > maxVal) return maxVal;
	return value;
}

// Function prototypes
void bannerAndInput(string& name, int& campers, int& nightsStaying, int& firesPlanned, char& ch);
void calculations(int campers, int nightsStaying, int firesPlanned, int& fireStarter,
	double& lbsMarshmallow);
void printSave(SupplyList& supply, Itin& itinerary, HikingSupplies& hikingSupplies, bool hasHikingSupplies, 
	int length, string name, int campers, int nightsStaying, int firesPlanned, int fireStarter, 
	double lbsMarshmallow, char ch, bool& raiseFlag);
void colorText();

// Helper functions for calculations (testable)
void calculateFireStarters(int firesPlanned, int nightsStaying, int& fireStarter)
{
	fireStarter = firesPlanned * nightsStaying;
}

void calculateMarshmallows(int fireStarter, int campers, double& lbsMarshmallow)
{
	lbsMarshmallow = (fireStarter * campers) / 4.0;
}

// ============= UNIT TESTS =============

// A) Calculations - exactly 4 tests (derived values, averages, ratios)
TEST_CASE("Calculations: Fire starters - normal case") {
	int fireStarter = 0;
	calculateFireStarters(2, 3, fireStarter);
	CHECK(fireStarter == 6);
}

TEST_CASE("Calculations: Fire starters - edge case with 0") {
	int fireStarter = 0;
	calculateFireStarters(0, 5, fireStarter);
	CHECK(fireStarter == 0);
}

TEST_CASE("Calculations: Marshmallow weight - normal case") {
	double lbsMarshmallow = 0.0;
	calculateMarshmallows(8, 4, lbsMarshmallow);
	CHECK(lbsMarshmallow == doctest::Approx(8.0));
}

TEST_CASE("Calculations: Average food quantity - guard divide by zero") {
	GeneralSupplies supply;
	CHECK(supply.getAverageFoodQuantity() == doctest::Approx(0.0));
}

// B) Enum decision logic - exactly 3 tests (if/switch based on enum)
TEST_CASE("Enum: Priority to string - low") {
	GeneralSupplies supply;
	CHECK(supply.getPriorityString(low) == "Low");
}

TEST_CASE("Enum: Priority to string - medium") {
	GeneralSupplies supply;
	CHECK(supply.getPriorityString(medium) == "Med");
}

TEST_CASE("Enum: Priority to string - high") {
	GeneralSupplies supply;
	CHECK(supply.getPriorityString(high) == "High");
}

// C) Struct/array processing - exactly 3 tests
TEST_CASE("Array: Add food items and count") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item1 = { "Rice", SupplyList::food, low, 2 };
	SupplyList::inventoryItem item2 = { "Pasta", SupplyList::food, medium, 3 };
	
	supply.addFoodItem(item1);
	supply.addFoodItem(item2);
	
	CHECK(supply.getFoodCount() == 2);
}

TEST_CASE("Array: Total items - food plus gear") {
	GeneralSupplies supply;
	SupplyList::inventoryItem food1 = { "Bread", SupplyList::food, low, 5 };
	SupplyList::inventoryItem gear1 = { "Rope", SupplyList::gear, medium, 2 };
	
	supply.addFoodItem(food1);
	supply.addGearItem(gear1);
	
	CHECK(supply.getTotalItemCount() == 2);
}

TEST_CASE("Array: Boundary - reject when full") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item = { "Item", SupplyList::food, low, 1 };
	
	// Fill to capacity
	for (int i = 0; i < MAX_ARRAY; i++) {
		supply.addFoodItem(item);
	}
	
	// Should fail when full
	CHECK(supply.addFoodItem(item) == false);
}

// D) Class methods - exactly 2 tests
TEST_CASE("Class: addFoodItem success") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item = { "Apples", SupplyList::food, medium, 10 };
	
	CHECK(supply.addFoodItem(item) == true);
	CHECK(supply.getFoodCount() == 1);
}

TEST_CASE("Class: clearAll resets") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item = { "Test", SupplyList::food, low, 1 };
	
	supply.addFoodItem(item);
	supply.clearAll();
	
	CHECK(supply.getFoodCount() == 0);
}

// ============= ADDITIONAL TESTS FOR NEW CLASSES =============

// E) Constructor and getter/setter tests
TEST_CASE("Menu: Constructor initializes fields correctly") {
	Menu testMenu("TestUser", 3, high);
	CHECK(testMenu.getUserName() == "TestUser");
	CHECK(testMenu.getNumOptions() == 3);
	CHECK(testMenu.getMenuPriority() == high);
}

TEST_CASE("Menu: Setters work correctly") {
	Menu testMenu;
	testMenu.setUserName("Alice");
	testMenu.setNumOptions(5);
	testMenu.setOption(0, "Option 1");
	
	CHECK(testMenu.getUserName() == "Alice");
	CHECK(testMenu.getNumOptions() == 5);
	CHECK(testMenu.getOption(0) == "Option 1");
}

TEST_CASE("Itin: Constructor initializes nights correctly") {
	Itin trip(5, high);
	CHECK(trip.getNightsStaying() == 5);
	CHECK(trip.getActivityCount() == 0);
}

TEST_CASE("Itin: addActivity increases count") {
	Itin trip(3, medium);
	trip.addActivity("Hiking", high);
	trip.addActivity("Fishing", low);
	
	CHECK(trip.getActivityCount() == 2);
	CHECK(trip.getActivity(0) == "Hiking");
	CHECK(trip.getActivity(1) == "Fishing");
}

// F) Derived class tests - HikingSupplies calls base behavior
TEST_CASE("HikingSupplies: Derives from SupplyList correctly") {
	HikingSupplies hiking("Trail Gear", 10, high, 2, 5.5);
	
	// Base class properties accessible
	CHECK(hiking.getListName() == "Trail Gear");
	CHECK(hiking.getMaxCapacity() == 10);
	CHECK(hiking.getListPriority() == high);
	
	// Derived class properties
	CHECK(hiking.getTrailDifficulty() == 2);
	CHECK(hiking.getDistanceMiles() == doctest::Approx(5.5));
}

TEST_CASE("HikingSupplies: addHikingEssentials uses base addItem methods") {
	HikingSupplies hiking("Test", 20, medium, 3, 10.0);
	hiking.addHikingEssentials();
	
	// Should have added items using base class methods
	CHECK(hiking.getFoodCount() > 0);  // At least water bottles + energy bars for difficulty 3
	CHECK(hiking.getGearCount() > 0);  // At least map, first aid, shelter, GPS for difficulty 3
	CHECK(hiking.getTotalItemCount() > 0);
}

// G) Derived class tests - LogSelect extends Menu 
TEST_CASE("LogSelect: Derives from Menu correctly") {
	LogSelect logMenu("TestUser");
	
	// Base class property accessible
	CHECK(logMenu.getUserName() == "TestUser");
	
	// Derived class property
	CHECK(logMenu.getSelectedLogNumber() == 0);  // Default value
}

// H) Composition class tests - Logs aggregates Itin and SupplyList
TEST_CASE("Logs: Composition - contains Itin and SupplyList") {
	Itin testItin(3, medium);
	testItin.addActivity("Kayaking", high);
	
	GeneralSupplies testSupply("Camp Gear", 10, medium);
	SupplyList::inventoryItem item = { "Tent", SupplyList::gear, high, 1 };
	testSupply.addGearItem(item);
	
	Logs testLog("Summer Trip", testItin, testSupply);
	
	// Logs contains copies of both objects
	CHECK(testLog.getLogName() == "Summer Trip");
	CHECK(testLog.getItinerary().getNightsStaying() == 3);
	CHECK(testLog.getItinerary().getActivityCount() == 1);
	CHECK(testLog.getSupplies().getGearCount() == 1);
}

TEST_CASE("Logs: Static counter persists across instances") {
	int initialCount = Logs::getTotalLogs();
	
	Itin itin1(2, low);
	GeneralSupplies supply1;
	Logs log1("Trip1", itin1, supply1);
	
	Itin itin2(3, high);
	GeneralSupplies supply2;
	Logs log2("Trip2", itin2, supply2);
	
	// Counter should be same for both (static member)
	CHECK(Logs::getTotalLogs() == initialCount);  // Not incremented unless saveLog() called
}

// ============= WEEK 05 TESTS - MANAGER CLASS & VIRTUAL FUNCTIONS =============

// I) Manager class tests - dynamic memory management
TEST_CASE("Manager: Default constructor initializes correctly") {
	SupplyManager manager;
	CHECK(manager.getSize() == 0);
	CHECK(manager.getCapacity() == 5);  // Default capacity
}

TEST_CASE("Manager: Parameterized constructor sets capacity") {
	SupplyManager manager(10);
	CHECK(manager.getSize() == 0);
	CHECK(manager.getCapacity() == 10);
}

TEST_CASE("Manager: Add supply increases size") {
	SupplyManager manager;
	
	// Create dynamic objects and add to manager
	GeneralSupplies* general = new GeneralSupplies("Camp Gear", 10, medium);
	HikingSupplies* hiking = new HikingSupplies("Trail Gear", 20, high, 2, 5.5);
	
	manager.addSupply(general);
	manager.addSupply(hiking);
	
	CHECK(manager.getSize() == 2);
}

TEST_CASE("Manager: Remove supply decreases size") {
	SupplyManager manager;
	
	manager.addSupply(new GeneralSupplies("Test1", 10, low));
	manager.addSupply(new GeneralSupplies("Test2", 10, medium));
	
	CHECK(manager.getSize() == 2);
	
	bool removed = manager.removeSupply(0);
	CHECK(removed == true);
	CHECK(manager.getSize() == 1);
}

TEST_CASE("Manager: Remove invalid index returns false") {
	SupplyManager manager;
	manager.addSupply(new GeneralSupplies("Test", 10, low));
	
	CHECK(manager.removeSupply(-1) == false);  // Invalid negative index
	CHECK(manager.removeSupply(5) == false);   // Out of bounds
	CHECK(manager.getSize() == 1);             // Size unchanged
}

TEST_CASE("Manager: RemoveAll clears all items") {
	SupplyManager manager;
	
	manager.addSupply(new GeneralSupplies("Test1", 10, low));
	manager.addSupply(new HikingSupplies("Test2", 20, high, 3, 10.0));
	manager.addSupply(new GeneralSupplies("Test3", 15, medium));
	
	CHECK(manager.getSize() == 3);
	
	manager.removeAll();
	CHECK(manager.getSize() == 0);
}

TEST_CASE("Manager: GetSupply returns correct pointer") {
	SupplyManager manager;
	
	GeneralSupplies* general = new GeneralSupplies("Camp Gear", 10, medium);
	manager.addSupply(general);
	
	SupplyList* retrieved = manager.getSupply(0);
	CHECK(retrieved != nullptr);
	CHECK(retrieved->getListName() == "Camp Gear");
}

TEST_CASE("Manager: GetSupply with invalid index returns nullptr") {
	SupplyManager manager;
	manager.addSupply(new GeneralSupplies("Test", 10, low));
	
	CHECK(manager.getSupply(-1) == nullptr);
	CHECK(manager.getSupply(10) == nullptr);
}

TEST_CASE("Manager: Auto-resize when capacity reached") {
	SupplyManager manager(2);  // Start with capacity of 2
	
	CHECK(manager.getCapacity() == 2);
	
	// Add 3 items - should trigger resize
	manager.addSupply(new GeneralSupplies("Item1", 10, low));
	manager.addSupply(new GeneralSupplies("Item2", 10, low));
	manager.addSupply(new GeneralSupplies("Item3", 10, low));
	
	CHECK(manager.getSize() == 3);
	CHECK(manager.getCapacity() == 4);  // Should have doubled from 2 to 4
}

// J) Virtual function tests - polymorphism
TEST_CASE("Virtual: getSupplyType returns correct type for GeneralSupplies") {
	GeneralSupplies general("Camp Gear", 10, medium);
	CHECK(general.getSupplyType() == "General Camping Supplies");
}

TEST_CASE("Virtual: getSupplyType returns correct type for HikingSupplies") {
	HikingSupplies hiking("Trail Gear", 20, high, 2, 5.5);
	CHECK(hiking.getSupplyType() == "Hiking Supplies - Moderate Trail");
}

TEST_CASE("Virtual: Polymorphic call through base pointer") {
	SupplyList* basePtr = new HikingSupplies("Trail", 20, high, 3, 12.0);
	
	// Should call HikingSupplies::getSupplyType() due to virtual function
	CHECK(basePtr->getSupplyType() == "Hiking Supplies - Difficult Trail");
	
	delete basePtr;  // Virtual destructor ensures proper cleanup
}

TEST_CASE("Virtual: Manager stores different types polymorphically") {
	SupplyManager manager;
	
	manager.addSupply(new GeneralSupplies("General", 10, low));
	manager.addSupply(new HikingSupplies("Hiking", 20, high, 1, 3.0));
	
	// Retrieve and check types through base pointer
	SupplyList* item0 = manager.getSupply(0);
	SupplyList* item1 = manager.getSupply(1);
	
	CHECK(item0->getSupplyType() == "General Camping Supplies");
	CHECK(item1->getSupplyType() == "Hiking Supplies - Easy Trail");
}

// ============= WEEK 06 TESTS - OPERATOR OVERLOADING =============

// K) Operator== tests - at least 2 tests
TEST_CASE("Operator==: Equal GeneralSupplies objects") {
	GeneralSupplies supply1("Camp Gear", 10, medium);
	GeneralSupplies supply2("Camp Gear", 10, medium);
	
	// Should be equal (same name, capacity, priority)
	CHECK(supply1 == supply2);
}

TEST_CASE("Operator==: Not equal GeneralSupplies objects - different name") {
	GeneralSupplies supply1("Camp Gear", 10, medium);
	GeneralSupplies supply2("Trail Gear", 10, medium);
	
	// Should NOT be equal (different name)
	CHECK_FALSE(supply1 == supply2);
}

TEST_CASE("Operator==: Not equal GeneralSupplies objects - different priority") {
	GeneralSupplies supply1("Camp Gear", 10, medium);
	GeneralSupplies supply2("Camp Gear", 10, high);
	
	// Should NOT be equal (different priority)
	CHECK_FALSE(supply1 == supply2);
}

// L) Operator<< tests - at least 2 tests (without ostringstream)
// We test that operator<< works by verifying the underlying data is correct
TEST_CASE("Operator<<: GeneralSupplies has correct data for output") {
	GeneralSupplies supply("Camp Gear", 10, medium);
	SupplyList::inventoryItem item1 = { "Tent", SupplyList::gear, high, 1 };
	SupplyList::inventoryItem item2 = { "Rope", SupplyList::gear, low, 2 };
	supply.addGearItem(item1);
	supply.addGearItem(item2);
	
	// Verify the data that would be output by operator<<
	CHECK(supply.getListName() == "Camp Gear");
	CHECK(supply.getPriorityString(supply.getListPriority()) == "Med");
	CHECK(supply.getTotalItemCount() == 2);
	CHECK(supply.getSupplyType() == "General Camping Supplies");
}

TEST_CASE("Operator<<: HikingSupplies has correct data for output") {
	HikingSupplies hiking("Trail Gear", 20, high, 3, 12.5);
	
	// Verify the data that would be output by operator<<
	CHECK(hiking.getListName() == "Trail Gear");
	CHECK(hiking.getDifficultyString() == "Difficult");
	CHECK(hiking.getDistanceMiles() == doctest::Approx(12.5));
	CHECK(hiking.getPriorityString(hiking.getListPriority()) == "High");
	CHECK(hiking.getSupplyType() == "Hiking Supplies - Difficult Trail");
}

TEST_CASE("Operator<<: Polymorphic behavior verified through base pointer") {
	SupplyList* basePtr = new GeneralSupplies("Test", 5, low);
	
	// Verify polymorphic toStream() is called correctly
	// by checking the supply type which uses virtual function
	CHECK(basePtr->getSupplyType() == "General Camping Supplies");
	CHECK(basePtr->getListName() == "Test");
	
	delete basePtr;
}

// M) Operator[] tests - at least 2 tests
TEST_CASE("Operator[]: Valid index returns correct item") {
	SupplyManager manager;
	GeneralSupplies* supply1 = new GeneralSupplies("First", 10, low);
	GeneralSupplies* supply2 = new GeneralSupplies("Second", 15, medium);
	
	manager.addSupply(supply1);
	manager.addSupply(supply2);
	
	// Access via operator[]
	SupplyList* retrieved = manager[1];
	CHECK(retrieved != nullptr);
	CHECK(retrieved->getListName() == "Second");
}

TEST_CASE("Operator[]: Invalid index throws exception") {
	SupplyManager manager;
	manager.addSupply(new GeneralSupplies("Test", 10, low));
	
	// Out of bounds access should throw
	CHECK_THROWS_AS(manager[-1], InvalidIndexException);
	CHECK_THROWS_AS(manager[10], InvalidIndexException);
}

TEST_CASE("Operator[]: Empty manager throws on any index") {
	SupplyManager manager;
	
	// No items in manager - any access should throw
	CHECK_THROWS_AS(manager[0], InvalidIndexException);
	CHECK_THROWS_AS(manager[-1], InvalidIndexException);
}

// N) Operator+= / Operator-= tests - at least 2 tests
TEST_CASE("Operator+=: Add increases size and stores correct pointer") {
	SupplyManager manager;
	GeneralSupplies* supply = new GeneralSupplies("Test", 10, medium);
	
	manager += supply;
	
	CHECK(manager.getSize() == 1);
	CHECK(manager[0]->getListName() == "Test");
}

TEST_CASE("Operator+=: Multiple adds work correctly") {
	SupplyManager manager;
	
	manager += new GeneralSupplies("First", 5, low);
	manager += new HikingSupplies("Second", 10, high, 2, 5.0);
	manager += new GeneralSupplies("Third", 15, medium);
	
	CHECK(manager.getSize() == 3);
	CHECK(manager[0]->getListName() == "First");
	CHECK(manager[1]->getListName() == "Second");
	CHECK(manager[2]->getListName() == "Third");
}

TEST_CASE("Operator-=: Remove deletes and shifts properly") {
	SupplyManager manager;
	
	manager += new GeneralSupplies("First", 10, low);
	manager += new GeneralSupplies("Second", 10, medium);
	manager += new GeneralSupplies("Third", 10, high);
	
	CHECK(manager.getSize() == 3);
	
	// Remove middle item
	manager -= 1;
	
	CHECK(manager.getSize() == 2);
	CHECK(manager[0]->getListName() == "First");
	CHECK(manager[1]->getListName() == "Third");  // Should have shifted
}

TEST_CASE("Operator-=: Invalid index does not affect size") {
	SupplyManager manager;
	manager += new GeneralSupplies("Test", 10, low);
	
	int initialSize = manager.getSize();
	// operator-= must throw on invalid removal
	CHECK_THROWS_AS(manager -= 10, InvalidIndexException);
	// size should remain unchanged after failed removal
	CHECK(manager.getSize() == initialSize);
}



TEST_CASE("Operator-=: Remove all items one by one") {
	SupplyManager manager;
	manager += new GeneralSupplies("First", 10, low);
	manager += new GeneralSupplies("Second", 10, medium);
	
	manager -= 0;
	CHECK(manager.getSize() == 1);
	
	manager -= 0;
	CHECK(manager.getSize() == 0);
}

TEST_CASE("Manager: printAllRecursive does not throw and preserves size") {
	SupplyManager manager;
	manager += new GeneralSupplies("First", 10, low);
	manager += new HikingSupplies("Trail Gear", 12, medium, 2, 5.0);

	CHECK(manager.getSize() == 2);
	CHECK_NOTHROW(manager.printAllRecursive()); // exercise the recursive member
	CHECK(manager.getSize() == 2);               // ensure printing didn't modify contents
}

// ============= WEEK 06 TESTS - TEMPLATE USAGE =============

// O) Function template tests - at least 2 tests
TEST_CASE("Template Function: maxValue with integers") {
	int a = 10;
	int b = 20;
	
	int result = maxValue(a, b);
	CHECK(result == 20);
	
	result = maxValue(b, a);
	CHECK(result == 20);
}

TEST_CASE("Template Function: maxValue with doubles") {
	double x = 3.14;
	double y = 2.71;
	
	double result = maxValue(x, y);
	CHECK(result == doctest::Approx(3.14));
}

TEST_CASE("Template Function: clamp with integers") {
	int value = 15;
	int minVal = 10;
	int maxVal = 20;
	
	CHECK(clamp(value, minVal, maxVal) == 15);  // Within range
	CHECK(clamp(5, minVal, maxVal) == 10);      // Below min
	CHECK(clamp(25, minVal, maxVal) == 20);     // Above max
}

TEST_CASE("Template Function: clamp with doubles") {
	double value = 5.5;
	double minVal = 0.0;
	double maxVal = 10.0;
	
	CHECK(clamp(value, minVal, maxVal) == doctest::Approx(5.5));
	CHECK(clamp(-1.0, minVal, maxVal) == doctest::Approx(0.0));
	CHECK(clamp(15.0, minVal, maxVal) == doctest::Approx(10.0));
}

// P) Class template tests - at least 2 tests
TEST_CASE("Template Class: DynamicArray stores and retrieves integers") {
	DynamicArray<int> arr;
	
	arr += 10;
	arr += 20;
	arr += 30;
	
	CHECK(arr.getSize() == 3);
	CHECK(arr[0] == 10);
	CHECK(arr[1] == 20);
	CHECK(arr[2] == 30);
}

TEST_CASE("Template Class: DynamicArray removes and shifts") {
	DynamicArray<int> arr;
	
	arr += 100;
	arr += 200;
	arr += 300;
	arr += 400;
	
	CHECK(arr.getSize() == 4);
	
	arr -= 1;  // Remove 200
	
	CHECK(arr.getSize() == 3);
	CHECK(arr[0] == 100);
	CHECK(arr[1] == 300);  // Shifted from index 2
	CHECK(arr[2] == 400);  // Shifted from index 3
}

TEST_CASE("Template Class: DynamicArray auto-resizes") {
	DynamicArray<int> arr(2);  // Start with capacity 2
	
	CHECK(arr.getCapacity() == 2);
	
	arr += 1;
	arr += 2;
	arr += 3;  // Should trigger resize
	
	CHECK(arr.getSize() == 3);
	CHECK(arr.getCapacity() == 4);  // Should have doubled
}

TEST_CASE("Template Class: DynamicArray with strings") {
	DynamicArray<string> arr;
	
	arr += "Hello";
	arr += "World";
	arr += "Test";
	
	CHECK(arr.getSize() == 3);
	CHECK(arr[0] == "Hello");
	CHECK(arr[1] == "World");
	CHECK(arr[2] == "Test");
}

TEST_CASE("Template Class: DynamicArray bounds checking") {
	DynamicArray<int> arr;
	arr += 42;
	
	// Valid access
	CHECK(arr[0] == 42);
	
	// Invalid access - should throw InvalidIndexException
	CHECK_THROWS_AS(arr[10], InvalidIndexException);           //****************************
}

// ============= WEEK 09 TESTS - VECTOR SEARCH & SORT =============

TEST_CASE("Search: Sequential - found and not found") {
	GeneralSupplies supply;
	SupplyList::inventoryItem i1 = { "Apple", SupplyList::food, low, 1 };
	SupplyList::inventoryItem i2 = { "Bread", SupplyList::food, medium, 2 };
	SupplyList::inventoryItem i3 = { "Carrot", SupplyList::food, high, 3 };

	CHECK(supply.addFoodItem(i1) == true);
	CHECK(supply.addFoodItem(i2) == true);
	CHECK(supply.addFoodItem(i3) == true);

	// Found cases
	CHECK(supply.sequentialSearchFood("Apple") == 0);
	CHECK(supply.sequentialSearchFood("Bread") == 1);
	CHECK(supply.sequentialSearchFood("Carrot") == 2);

	// Not found
	CHECK(supply.sequentialSearchFood("Zucchini") == -1);
}

TEST_CASE("Sort and Binary Search: insertion sort then binary search") {
	GeneralSupplies supply;
	SupplyList::inventoryItem a = { "Water", SupplyList::food, low, 1 };
	SupplyList::inventoryItem b = { "Apple", SupplyList::food, low, 1 };
	SupplyList::inventoryItem c = { "Banana", SupplyList::food, low, 1 };

	CHECK(supply.addFoodItem(a) == true);
	CHECK(supply.addFoodItem(b) == true);
	CHECK(supply.addFoodItem(c) == true);

	// Confirm initial (unsorted) order
	CHECK(supply.getFoodItem(0).itemName == "Water");

	// Sort and verify order
	supply.insertionSortFoodByName();
	CHECK(supply.getFoodItem(0).itemName == "Apple");
	CHECK(supply.getFoodItem(1).itemName == "Banana");
	CHECK(supply.getFoodItem(2).itemName == "Water");

	// Binary search finds items (requires sorted list)
	int idx = supply.binarySearchFoodByName("Banana");
	CHECK(idx != -1);
	CHECK(supply.getFoodItem(idx).itemName == "Banana");

	// Not found case for binary search
	CHECK(supply.binarySearchFoodByName("Z") == -1);
}

TEST_CASE("Edge: empty lists search behavior") {
	GeneralSupplies supply;

	// Empty list should return -1 for both searches
	CHECK(supply.sequentialSearchFood("Anything") == -1);
	CHECK(supply.binarySearchFoodByName("Anything") == -1);
}

TEST_CASE("Duplicates: sequential returns first occurrence; sort is stable") {
	GeneralSupplies supply;
	SupplyList::inventoryItem x = { "Grape", SupplyList::food, low, 1 };
	SupplyList::inventoryItem y = { "Grape", SupplyList::food, low, 2 };

	CHECK(supply.addFoodItem(x) == true);
	CHECK(supply.addFoodItem(y) == true);

	// Sequential search returns first occurrence
	CHECK(supply.sequentialSearchFood("Grape") == 0);

	// Sort should preserve relative order for equal keys (stable)
	supply.insertionSortFoodByName();
	CHECK(supply.getFoodItem(0).itemName == "Grape");
	CHECK(supply.getFoodItem(1).itemName == "Grape");
	CHECK(supply.getFoodItem(0).quantity == 1); // first entry remained first

	// Binary search finds a matching index
	int idx = supply.binarySearchFoodByName("Grape");
	CHECK(idx != -1);
	CHECK(supply.getFoodItem(idx).itemName == "Grape");
}

// ============= WEEK 10 TESTS - LINKED LIST ADT =============

// Q) LinkedList basic operations - empty list and insertion
TEST_CASE("LinkedList: Insert into empty list") {
	LinkedList<int> list;

	// Empty list checks
	CHECK(list.empty() == true);
	CHECK(list.size() == 0);

	// Insert one item
	list.insertBack(42);

	CHECK(list.empty() == false);
	CHECK(list.size() == 1);
	CHECK(list.at(0) == 42);
}

TEST_CASE("LinkedList: Insert at front and back (unordered)") {
	LinkedList<string> list;

	// Demonstrate TWO insertion positions (unordered requirement)
	list.insertBack("World");   // Back insertion
	list.insertFront("Hello");  // Front insertion
	list.insertBack("!");       // Back insertion

	CHECK(list.size() == 3);
	CHECK(list.at(0) == "Hello");  // Front insertion placed this first
	CHECK(list.at(1) == "World");  // Original item shifted
	CHECK(list.at(2) == "!");      // Back insertion at end
}

TEST_CASE("LinkedList: Multiple insertFront operations") {
	LinkedList<int> list;

	list.insertFront(30);
	list.insertFront(20);
	list.insertFront(10);

	// Front insertion reverses order: last inserted becomes first
	CHECK(list.size() == 3);
	CHECK(list.at(0) == 10);
	CHECK(list.at(1) == 20);
	CHECK(list.at(2) == 30);
}

// R) LinkedList deletion operations
TEST_CASE("LinkedList: Delete existing node") {
	LinkedList<int> list;
	list.insertBack(10);
	list.insertBack(20);
	list.insertBack(30);

	CHECK(list.size() == 3);

	// Delete middle item
	bool deleted = list.deleteNode(20);

	CHECK(deleted == true);
	CHECK(list.size() == 2);
	CHECK(list.at(0) == 10);
	CHECK(list.at(1) == 30);  // Items shifted after deletion
}

TEST_CASE("LinkedList: Delete non-existent node") {
	LinkedList<int> list;
	list.insertBack(10);
	list.insertBack(20);

	// Try to delete item not in list
	bool deleted = list.deleteNode(99);

	CHECK(deleted == false);
	CHECK(list.size() == 2);  // Size unchanged
}

TEST_CASE("LinkedList: Delete first node") {
	LinkedList<int> list;
	list.insertBack(10);
	list.insertBack(20);
	list.insertBack(30);

	bool deleted = list.deleteNode(10);  // Delete first

	CHECK(deleted == true);
	CHECK(list.size() == 2);
	CHECK(list.at(0) == 20);  // New first element
}

TEST_CASE("LinkedList: Delete last node") {
	LinkedList<int> list;
	list.insertBack(10);
	list.insertBack(20);
	list.insertBack(30);

	bool deleted = list.deleteNode(30);  // Delete last

	CHECK(deleted == true);
	CHECK(list.size() == 2);
	CHECK(list.at(1) == 20);  // Last element now
}

TEST_CASE("LinkedList: Delete from single-item list") {
	LinkedList<int> list;
	list.insertBack(42);

	bool deleted = list.deleteNode(42);

	CHECK(deleted == true);
	CHECK(list.empty() == true);
	CHECK(list.size() == 0);
}

// S) LinkedList search operations
TEST_CASE("LinkedList: Search in empty list") {
	LinkedList<int> list;

	// Searching empty list should return false
	CHECK(list.search(42) == false);
}

TEST_CASE("LinkedList: Search for existing item") {
	LinkedList<string> list;
	list.insertBack("Apple");
	list.insertBack("Banana");
	list.insertBack("Cherry");

	CHECK(list.search("Banana") == true);
	CHECK(list.search("Apple") == true);
	CHECK(list.search("Cherry") == true);
}

TEST_CASE("LinkedList: Search for non-existing item") {
	LinkedList<string> list;
	list.insertBack("Apple");
	list.insertBack("Banana");

	CHECK(list.search("Grape") == false);
	CHECK(list.search("Orange") == false);
}

// T) LinkedList iterator operations
TEST_CASE("LinkedList: Traverse empty list with iterator") {
	LinkedList<int> list;

	LinkedListIterator<int> iter = list.begin();

	// Iterator should equal end() for empty list
	CHECK(iter == list.end());
}

TEST_CASE("LinkedList: Iterator traversal and data access") {
	LinkedList<int> list;
	list.insertBack(1);
	list.insertBack(2);
	list.insertBack(3);

	LinkedListIterator<int> iter = list.begin();
	int count = 0;
	int sum = 0;

	// Traverse using iterator
	while (iter != list.end())
	{
		sum += *iter;  // Dereference operator
		count++;
		++iter;        // Pre-increment operator
	}

	CHECK(count == 3);
	CHECK(sum == 6);
}

TEST_CASE("LinkedList: Iterator with struct data") {
	LinkedList<SupplyList::inventoryItem> list;

	SupplyList::inventoryItem item1 = { "Tent", SupplyList::gear, high, 1 };
	SupplyList::inventoryItem item2 = { "Rope", SupplyList::gear, medium, 2 };

	list.insertBack(item1);
	list.insertBack(item2);

	LinkedListIterator<SupplyList::inventoryItem> iter = list.begin();

	// Test arrow operator for accessing struct members
	CHECK(iter->itemName == "Tent");
	CHECK(iter->quantity == 1);

	++iter;
	CHECK(iter->itemName == "Rope");
	CHECK(iter->quantity == 2);
}

// U) LinkedList copy operations
TEST_CASE("LinkedList: Copy constructor creates deep copy") {
	LinkedList<int> list1;
	list1.insertBack(10);
	list1.insertBack(20);
	list1.insertBack(30);

	// Copy constructor
	LinkedList<int> list2(list1);

	CHECK(list2.size() == 3);
	CHECK(list2.at(0) == 10);
	CHECK(list2.at(1) == 20);
	CHECK(list2.at(2) == 30);

	// Modify list1 - should not affect list2 (deep copy)
	list1.deleteNode(10);

	CHECK(list1.size() == 2);
	CHECK(list2.size() == 3);  // list2 unchanged
}

TEST_CASE("LinkedList: Assignment operator") {
	LinkedList<int> list1;
	list1.insertBack(5);
	list1.insertBack(15);
	list1.insertBack(25);

	LinkedList<int> list2;
	list2.insertBack(99);  // list2 has different data

	// Assignment operator
	list2 = list1;

	CHECK(list2.size() == 3);
	CHECK(list2.at(0) == 5);
	CHECK(list2.at(1) == 15);
	CHECK(list2.at(2) == 25);

	// Modify list1 - should not affect list2
	list1.insertBack(35);

	CHECK(list1.size() == 4);
	CHECK(list2.size() == 3);  // list2 unchanged
}

TEST_CASE("LinkedList: Self-assignment") {
	LinkedList<int> list;
	list.insertBack(1);
	list.insertBack(2);

	// Self-assignment should not crash
	list = list;

	CHECK(list.size() == 2);
	CHECK(list.at(0) == 1);
	CHECK(list.at(1) == 2);
}

// V) LinkedList clear and memory management
TEST_CASE("LinkedList: Clear removes all elements") {
	LinkedList<int> list;
	list.insertBack(1);
	list.insertBack(2);
	list.insertBack(3);

	CHECK(list.size() == 3);

	list.clear();

	CHECK(list.empty() == true);
	CHECK(list.size() == 0);
}

TEST_CASE("LinkedList: Clear on empty list") {
	LinkedList<int> list;

	// Clearing empty list should not crash
	list.clear();

	CHECK(list.empty() == true);
	CHECK(list.size() == 0);
}

// W) LinkedList array-style access
TEST_CASE("LinkedList: Array-style access with at()") {
	LinkedList<int> list;
	list.insertBack(100);
	list.insertBack(200);
	list.insertBack(300);

	CHECK(list.at(0) == 100);
	CHECK(list.at(1) == 200);
	CHECK(list.at(2) == 300);
}

TEST_CASE("LinkedList: at() throws on invalid index") {
	LinkedList<int> list;
	list.insertBack(42);

	// Valid access
	CHECK(list.at(0) == 42);

	// Invalid access should throw
	CHECK_THROWS_AS(list.at(-1), out_of_range);
	CHECK_THROWS_AS(list.at(5), out_of_range);
}

TEST_CASE("LinkedList: operator[] access") {
	LinkedList<string> list;
	list.insertBack("First");
	list.insertBack("Second");
	list.insertBack("Third");

	CHECK(list[0] == "First");
	CHECK(list[1] == "Second");
	CHECK(list[2] == "Third");
}

TEST_CASE("LinkedList: Modify elements via operator[]") {
	LinkedList<int> list;
	list.insertBack(10);
	list.insertBack(20);

	// Modify through operator[]
	list[0] = 99;
	list[1] = 88;

	CHECK(list[0] == 99);
	CHECK(list[1] == 88);
}

// X) SupplyList integration with LinkedList
TEST_CASE("SupplyList with LinkedList: Add and count items") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item1 = { "Rice", SupplyList::food, low, 2 };
	SupplyList::inventoryItem item2 = { "Pasta", SupplyList::food, medium, 3 };

	supply.addFoodItem(item1);
	supply.addFoodItem(item2);

	CHECK(supply.getFoodCount() == 2);
	CHECK(supply.getTotalItemCount() == 2);
}

TEST_CASE("SupplyList with LinkedList: Sequential search") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item1 = { "Bread", SupplyList::food, low, 5 };
	SupplyList::inventoryItem item2 = { "Cheese", SupplyList::food, medium, 3 };
	SupplyList::inventoryItem item3 = { "Apples", SupplyList::food, high, 10 };

	supply.addFoodItem(item1);
	supply.addFoodItem(item2);
	supply.addFoodItem(item3);

	// Search for existing items
	CHECK(supply.sequentialSearchFood("Cheese") == 1);
	CHECK(supply.sequentialSearchFood("Bread") == 0);
	CHECK(supply.sequentialSearchFood("Apples") == 2);

	// Search for non-existing item
	CHECK(supply.sequentialSearchFood("Pizza") == -1);
}

TEST_CASE("SupplyList with LinkedList: Clear all items") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item = { "Item", SupplyList::food, low, 1 };

	supply.addFoodItem(item);
	supply.addGearItem(item);

	CHECK(supply.getFoodCount() == 1);
	CHECK(supply.getGearCount() == 1);

	supply.clearAll();

	CHECK(supply.getFoodCount() == 0);
	CHECK(supply.getGearCount() == 0);
}

TEST_CASE("SupplyList with LinkedList: Average quantity calculation") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item1 = { "Item1", SupplyList::food, low, 10 };
	SupplyList::inventoryItem item2 = { "Item2", SupplyList::food, medium, 20 };
	SupplyList::inventoryItem item3 = { "Item3", SupplyList::food, high, 30 };

	supply.addFoodItem(item1);
	supply.addFoodItem(item2);
	supply.addFoodItem(item3);

	// Average = (10 + 20 + 30) / 3 = 20.0
	CHECK(supply.getAverageFoodQuantity() == doctest::Approx(20.0));
}

TEST_CASE("SupplyList with LinkedList: Reach capacity limit") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item = { "Item", SupplyList::food, low, 1 };

	// Fill to MAX_ARRAY capacity
	for (int i = 0; i < MAX_ARRAY; i++)
	{
		CHECK(supply.addFoodItem(item) == true);
	}

	CHECK(supply.getFoodCount() == MAX_ARRAY);

	// Try to add one more - should fail
	CHECK(supply.addFoodItem(item) == false);
	CHECK(supply.getFoodCount() == MAX_ARRAY);  // Size unchanged
}

TEST_CASE("SupplyList with LinkedList: Access items by index") {
	GeneralSupplies supply;
	SupplyList::inventoryItem item1 = { "First", SupplyList::food, low, 1 };
	SupplyList::inventoryItem item2 = { "Second", SupplyList::food, medium, 2 };

	supply.addFoodItem(item1);
	supply.addFoodItem(item2);

	const SupplyList::inventoryItem& retrieved1 = supply.getFoodItem(0);
	const SupplyList::inventoryItem& retrieved2 = supply.getFoodItem(1);

	CHECK(retrieved1.itemName == "First");
	CHECK(retrieved1.quantity == 1);
	CHECK(retrieved2.itemName == "Second");
	CHECK(retrieved2.quantity == 2);
}

// ============= MAIN PROGRAM =============
int main(int argc, char** argv)
{
	// Instantiate DebugMemoryCheck immediately so it snapshots before any test or program activity.
	// In Debug builds this will report leaks at program exit; in Release it's a no-op.
	DebugMemoryCheck dbgMemCheck;

	// Check if we should run tests (Debug mode or if --test flag is passed)
#if defined(_DEBUG) || defined(RUN_TESTS)
    return doctest::Context(argc, argv).run();
#else
	// In release mode, check command line for test flag
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--test") == 0) {
			return doctest::Context(argc, argv).run();
		}
	}
	
	// Otherwise run normal camping list generator
	string name;
	string extraItem;
	char ch;
	int campers;
	int nightsStaying;
	int fireStarter;
	double lbsMarshmallow;
	int firesPlanned;
	bool raiseFlag = false;
	GeneralSupplies supply;

	colorText();

	// Load log count at program start
	Logs::loadLogCount();

	// Option to view previous logs using LogSelect
	cout << "# Would you like to view previous trip logs? (Y/N) #" << endl;
	char viewLogs;
	cin >> viewLogs;
	cin.ignore(50, '\n');

	if (viewLogs == 'Y' || viewLogs == 'y')
	{
		LogSelect logMenu("Guest");
		logMenu.selectLog();
		cout << endl;
		
		// Clear input stream before proceeding to bannerAndInput
		cin.clear();
		cin.ignore(50, '\n');
	}

	bannerAndInput(name, campers, nightsStaying, firesPlanned, ch);
	
	// Create Itin object with nightsStaying from user input
	Itin itinerary(nightsStaying, medium);
	
	supply.extrasFunc(ch, MAX_ARRAY);
	
	// Ask about hiking supplies (optional) - stack allocated
	char hikingChoice;
	HikingSupplies hikingSupplies;  // Default constructed on stack
	bool hasHikingSupplies = false;
	
	cout << endl << "# Will this trip involve hiking? (Y/N) #" << endl;
	cin >> hikingChoice;
	cin.ignore(50, '\n');
	
	if (hikingChoice == 'Y' || hikingChoice == 'y')
	{
		int difficulty;
		double distance;
		
		cout << "# Enter trail difficulty (1=Easy, 2=Moderate, 3=Difficult): ";
		cin >> difficulty;
		cin.ignore(50, '\n');
		
		cout << "# Enter distance in miles: ";
		cin >> distance;
		cin.ignore(50, '\n');
		
		// Set hiking supplies properties
		hikingSupplies = HikingSupplies("Hiking Gear", MAX_ARRAY, high, difficulty, distance);
		hikingSupplies.addHikingEssentials();
		hasHikingSupplies = true;
		
		cout << endl;
	}
	
	calculations(campers, nightsStaying, firesPlanned, fireStarter,
		lbsMarshmallow);
	printSave(supply, itinerary, hikingSupplies, hasHikingSupplies, MAX_ARRAY, name, campers, nightsStaying,
		firesPlanned, fireStarter, lbsMarshmallow, ch, raiseFlag);

	if (raiseFlag)
	{
		return 1;
	}

	// Save trip as a Log before exit
	cout << endl << "# Would you like to save this trip as a log? (Y/N) #" << endl;
	char saveLog;
	cin >> saveLog;
	cin.ignore(50, '\n');

	if (saveLog == 'Y' || saveLog == 'y')
	{
		string logName;
		cout << "# Enter a name for this trip log: ";
		getline(cin, logName);

		Logs tripLog(logName, itinerary, supply);
		tripLog.saveLog();
		cout << endl;
	}

	cout << "# Thanks for allowing me to assist you  #" << endl
		<< "# with planning! I hope you found this  #" << endl
		<< "# tool to be helpful - enjoy your trip! #"
		<< endl << endl;

	return 0;
#endif
}

void bannerAndInput(string& name, int& campers, int& nightsStaying, int& firesPlanned, char& ch)
{
	cout << "#=======================================#" << endl
		<< "#===== Welcome! You are now using ======#" << endl
		<< "# Shane's Camping Supply List Generator #" << endl
		<< "# Allow me to take some information to  #" << endl
		<< "#  help plan your trip - to begin, what #" << endl
		<< "#======================= is your name ? #" << endl << endl;

	cin.clear();
	cin.ignore(50, '\n'); // ensure clean input
	getline(cin, name);

	cout << endl << endl << "#== Okay, " << left << setfill('=') 
		<< setw(25) << name + "  " << "=====#" << endl
		<< "#== and how many campers will be going, #" << endl
		<< "#=================  including yourself? #" << endl << endl;

	if (!(cin >> campers)) {
		campers = 1;
		cout << "#== The value needs to be an integer! ==#" << endl
			<< "#==== = = - -  I've set the value to 1 =#" << endl << endl;
	}

	cout << endl << "# " << campers << " Campers, okay! Now, how many nights #" << endl
		<< "#======= = - - -   will you be staying? #" << endl << endl;

	cin.clear();
	cin.ignore(50, '\n');

	if (!(cin >> nightsStaying)) {
		nightsStaying = 1;
		cout << "#== The value needs to be an integer! ==#" << endl
			<< "#==== = = - -  I've set the value to 1 =#" << endl << endl;
	}

	cout << endl << "# " << nightsStaying << " Nights, okay! Now, how many fires ==#" << endl
		<< "#==== - do you plan to have each day? ==#" << endl;

	cin.clear();
	cin.ignore(50, '\n');

	if (!(cin >> firesPlanned)) {
		firesPlanned = 2;
		cout << "#== The value needs to be an integer! ==#" << endl
			<< "#==== = = - -  I've set the value to 2  =#" << endl << endl;
	}

	cout << endl << "# " << firesPlanned << " Campfires/day... Alright, now...  ==#"
		<< endl << "# are there any other items you would ==#" << endl
		<< "# like to add to your camping list?  ===#" << endl
		<< "#==== = = - - - -    Enter Y for yes ===#" << endl << endl;

	cin.clear();
	cin.ignore(50, '\n');

	cin >> ch;
}

void calculations(int campers, int nightsStaying, int firesPlanned, int& fireStarter,
	double& lbsMarshmallow)
{
	calculateFireStarters(firesPlanned, nightsStaying, fireStarter);
	calculateMarshmallows(fireStarter, campers, lbsMarshmallow);
}

void printSave(SupplyList& supply, Itin& itinerary, HikingSupplies& hikingSupplies, bool hasHikingSupplies,
	int length, string name, int campers, int nightsStaying, int firesPlanned, int fireStarter, 
	double lbsMarshmallow, char ch, bool& raiseFlag)
{
	// Use Menu class instead of menuSelect() function
	Menu tripMenu(name, 3, high);
	tripMenu.setOption(0, "Make/Replace Itinerary");
	tripMenu.setOption(1, "Save and display on screen");
	tripMenu.setOption(2, "Save to file only");

	int menu;
	string activity;
	string extraItem;
	bool hasItinerary = false;
	ofstream outData;
	ifstream inData;

	do {
		tripMenu.print();
		menu = tripMenu.getSelection();

		switch (menu) {
		case 1:
			// Build itinerary using Itin class
			itinerary.buildItinerary();
			itinerary.writeToFile(it);  // Write to legacy It.txt
			itinerary.writeIndexedFile();  // Auto-increments and wraps at 10
			hasItinerary = true;
			break;

		case 2:
			cout << "#=====   Printing list to screen   =====#" << endl << endl << endl;
			if (hasItinerary)
			{
				inData.open(it);

				if (!inData)
				{
					raiseFlag = true;
					cout << "#== Cannot open file, exiting program ==#" << endl << endl;
					return;
				}

				cout << "#*********************************#" << endl
					<< right << setw(19) << setfill('.') << name
					<< left << setw(16) << "'s Itinerary" << endl
					<< right << "#*********************************#" << endl;

				while (inData)
				{
					getline(inData, activity);
					cout << activity << endl;
				}
				inData.close();
			}
			cout << "#*********************************#" << endl;
			cout << right << setw(19) << setfill('.') << name
				<< left << "'s Camp Supplies" << endl
				<< showpoint << fixed << setprecision(2)
				<< setw(28) << "Size of camping party:" 
				<< right << setw(7) << campers << endl
				<< left << setw(28) << "Duration of stay:"
				<< right << setw(7) << nightsStaying << endl
				<< setfill('-') << "#" << setw(34) << "#" << endl
				<< "#" << setw(34) << "#" << endl
				<< setfill('.') << "#" << setw(16) << "Things "
				<< left << setw(17) << "to pack" << "#" << endl
				<< "Need: " << static_cast<int>((campers / 2.0) + .5)
				<< "x 2-man OR " << static_cast<int>((campers / 5.0) + .8)
				<< "x 5-man tent(s)" << endl
				<< setw(28) << "- - - Sleeping bags"
				<< right << setw(7) << campers << endl
				<< left << setw(28) << "- - - Fire starters"
				<< right << setw(7) << fireStarter << endl
				<< left << setw(28) << "- - - Lbs of Marshmallow"
				<< right << setw(7) << lbsMarshmallow << endl;
			if (ch == 'y' || ch == 'Y')
			{
				supply.printItems(length);
				if (raiseFlag)
				{
					return;
				}
			}
			// Add hiking supplies to screen output
			if (hasHikingSupplies)
			{
				hikingSupplies.printItems(length);
			}
			cout << "#" << setfill('-') << setw(34) << "#" << endl
				<< setfill('.') << setw(26) << "And lots of camping "
				<< left << "spirit!!!" << endl
				<< "#*********************************#"
				<< endl << endl << endl;

		case 3:
			outData.open(rpt);
			cout << "#======    Saving your report now...  ==#" << endl;

			if (hasItinerary)
			{
				inData.open(it);

				if (!inData)
				{
					raiseFlag = true;
					cout << "#== Cannot open file, exiting program ==#" << endl << endl;
					return;
				}

				outData << "#*********************************#" << endl
					<< setw(19) << setfill('.') << name
					<< left << setw(17) << "'s Itinerary" << endl
					<< right << "#*********************************#" << endl;

				while (inData)
				{
					getline(inData, activity);
					outData << activity << endl;
				}
				inData.close();
			}
			outData << "#*********************************#" << endl;
			outData << setw(19) << setfill('.') << name
				<< left << "'s Camp Supplies" << endl
				<< showpoint << fixed << setprecision(2)
				<< setw(28) << "Size of camping party:"
				<< right << setw(7) << campers << endl
				<< left << setw(28) << "Duration of stay:"
				<< right << setw(7) << nightsStaying << endl
				<< setfill('-') << "#" << setw(34) << "#" << endl
				<< "#" << setw(34) << "#" << endl
				<< setfill('.') << "#" << setw(16) << "Things "
				<< left << setw(17) << "to pack" << "#" << endl
				<< "Need: " << static_cast<int>((campers / 2.0) + .5)
				<< "x 2-man OR " << static_cast<int>((campers / 5.0) + .8)
				<< "x 5-man tent(s)" << endl
				<< setw(28) << "- - - Sleeping bags"
				<< right << setw(7) << campers << endl
				<< left << setw(28) << "- - - Fire starters"
				<< right << setw(7) << fireStarter << endl
				<< left << setw(28) << "- - - Lbs of Marshmallow"
				<< right << setw(7) << lbsMarshmallow << endl;
			if (ch == 'y' || ch == 'Y')
			{
				supply.addItems(outData, raiseFlag, length);
				if (raiseFlag)
				{
					return;
				}
			}
			// Add hiking supplies to file output
			if (hasHikingSupplies)
			{
				outData << endl << "#=== Hiking-Specific Supply List ===#" << endl;
				outData << "Trail Difficulty: " << hikingSupplies.getDifficultyString() << endl;
				outData << "Distance: " << fixed << setprecision(1) << hikingSupplies.getDistanceMiles() << " miles" << endl;
				outData << "Food items: " << hikingSupplies.getFoodCount() << endl;
				outData << "Gear items: " << hikingSupplies.getGearCount() << endl;
			}
			outData << "#" << setfill('-') << setw(34) << "#" << endl
				<< setfill('.') << setw(26) << "And lots of camping "
				<< left << "spirit!!!" << endl
				<< "#*********************************#"
				<< endl << endl << endl;

			cout << "#========      Save complete!     ======#" << endl;
			break;

		default:
			cout << "#  Invalid menu selection - no output   #" << endl
				<< "# will be generated. Please restart the #" << endl
				<< "#============     program to try again. #" << endl;
			raiseFlag = true;
			return;
		}
	} while (menu == 1);
}

void colorText()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COLOR);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu