// Main program: gathers trip info, uses SupplyList to collect extras, and prints/saves report
#if defined(_DEBUG) || defined(RUN_TESTS)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
#include "doctest.h"
#include "SupplyList.h"  // SupplyList class definition
#include "Itin.h"        // Itin class definition
#include "HikingSupplies.h"  // HikingSupplies class definition
#include "Menu.h"        // Menu class definition
#include "Logs.h"        // Logs class definition
#include "LogSelect.h"   // LogSelect class definition

const string extras = "Extras.txt"; // legacy filenames
const string it = "It.txt";         // itinerary file
const string rpt = "Report.txt";    // report output file
const int COLOR = 100;              // console color attribute

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
	SupplyList supply;
	CHECK(supply.getAverageFoodQuantity() == doctest::Approx(0.0));
}

// B) Enum decision logic - exactly 3 tests (if/switch based on enum)
TEST_CASE("Enum: Priority to string - low") {
	SupplyList supply;
	CHECK(supply.getPriorityString(low) == "Low");
}

TEST_CASE("Enum: Priority to string - medium") {
	SupplyList supply;
	CHECK(supply.getPriorityString(medium) == "Med");
}

TEST_CASE("Enum: Priority to string - high") {
	SupplyList supply;
	CHECK(supply.getPriorityString(high) == "High");
}

// C) Struct/array processing - exactly 3 tests
TEST_CASE("Array: Add food items and count") {
	SupplyList supply;
	SupplyList::inventoryItem item1 = { "Rice", SupplyList::food, low, 2 };
	SupplyList::inventoryItem item2 = { "Pasta", SupplyList::food, medium, 3 };
	
	supply.addFoodItem(item1);
	supply.addFoodItem(item2);
	
	CHECK(supply.getFoodCount() == 2);
}

TEST_CASE("Array: Total items - food plus gear") {
	SupplyList supply;
	SupplyList::inventoryItem food1 = { "Bread", SupplyList::food, low, 5 };
	SupplyList::inventoryItem gear1 = { "Rope", SupplyList::gear, medium, 2 };
	
	supply.addFoodItem(food1);
	supply.addGearItem(gear1);
	
	CHECK(supply.getTotalItemCount() == 2);
}

TEST_CASE("Array: Boundary - reject when full") {
	SupplyList supply;
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
	SupplyList supply;
	SupplyList::inventoryItem item = { "Apples", SupplyList::food, medium, 10 };
	
	CHECK(supply.addFoodItem(item) == true);
	CHECK(supply.getFoodCount() == 1);
}

TEST_CASE("Class: clearAll resets") {
	SupplyList supply;
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
	
	SupplyList testSupply("Camp Gear", 10, medium);
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
	SupplyList supply1;
	Logs log1("Trip1", itin1, supply1);
	
	Itin itin2(3, high);
	SupplyList supply2;
	Logs log2("Trip2", itin2, supply2);
	
	// Counter should be same for both (static member)
	CHECK(Logs::getTotalLogs() == initialCount);  // Not incremented unless saveLog() called
}

// ============= MAIN PROGRAM (only in program mode) =============

#if !defined(_DEBUG) && !defined(RUN_TESTS)
int main()
{
	// Program mode: run normal camping list generator
	string name;
	string extraItem;
	char ch;
	int campers;
	int nightsStaying;
	int fireStarter;
	double lbsMarshmallow;
	int firesPlanned;
	bool raiseFlag = false;
	SupplyList supply;

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
}
#endif

void bannerAndInput(string& name, int& campers, int& nightsStaying, int& firesPlanned, char& ch)
{
	cout << "#=======================================#" << endl
		<< "#===== Welcome! You are now using ======#" << endl
		<< "# Shane's Camping Supply List Generator #" << endl
		<< "# Allow me to take some information to  #" << endl
		<< "#  help plan your trip - to begin, what #" << endl
		<< "#======================= is your name ? #" << endl << endl;

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