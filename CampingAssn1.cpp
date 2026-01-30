// Main program: gathers trip info, uses SupplyList to collect extras, and prints/saves report
#if defined(_DEBUG) || defined(RUN_TESTS)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif
#include "doctest.h"
#include "SupplyList.h"

const string extras = "Extras.txt"; // legacy filenames
const string it = "It.txt";         // itinerary file
const string rpt = "Report.txt";    // report output file
const int COLOR = 100;              // console color attribute

// Function prototypes
void bannerAndInput(string& name, int& campers, int& nightsStaying, int& firesPlanned, char& ch);
void calculations(int campers, int nightsStaying, int firesPlanned, int& fireStarter,
	double& lbsMarshmallow);
int menuSelect();
void printSave(SupplyList& supply, int length, string name,
	int campers, int nightsStaying, int firesPlanned, int fireStarter, double lbsMarshmallow, char ch, bool& raiseFlag);
void itFunc(int nightsStaying, string activity, bool& itinerary);
void colorText();

// Helper functions for calculations (testable)
void calculateFireStarters(int firesPlanned, int nightsStaying, int& fireStarter)
{
	//initialize fireStarter variable; set # of fire starters to bring = fires
	//planned per day times the # of nights the user plans to be camping
	fireStarter = firesPlanned * nightsStaying;
}

void calculateMarshmallows(int fireStarter, int campers, double& lbsMarshmallow)
{
	//initialize lbsMarshmallow variable; set weight of marshmallow to bring = total # 
	//of campfires planned during the user's stay times the number of campers; then 
	//divide by 4, this assumes each person will consume .25 lbs of marshmallow per fire
	lbsMarshmallow = (fireStarter * campers) / 4.0;
	//lbsMarshmallow should be appropriate for the "derived value" requirement
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
	inventoryItem item1 = { "Rice", food, low, 2 };
	inventoryItem item2 = { "Pasta", food, medium, 3 };
	
	supply.addFoodItem(item1);
	supply.addFoodItem(item2);
	
	CHECK(supply.getFoodCount() == 2);
}

TEST_CASE("Array: Total items - food plus gear") {
	SupplyList supply;
	inventoryItem food1 = { "Bread", food, low, 5 };
	inventoryItem gear1 = { "Rope", gear, medium, 2 };
	
	supply.addFoodItem(food1);
	supply.addGearItem(gear1);
	
	CHECK(supply.getTotalItemCount() == 2);
}

TEST_CASE("Array: Boundary - reject when full") {
	SupplyList supply;
	inventoryItem item = { "Item", food, low, 1 };
	
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
	inventoryItem item = { "Apples", food, medium, 10 };
	
	CHECK(supply.addFoodItem(item) == true);
	CHECK(supply.getFoodCount() == 1);
}

TEST_CASE("Class: clearAll resets") {
	SupplyList supply;
	inventoryItem item = { "Test", food, low, 1 };
	
	supply.addFoodItem(item);
	supply.clearAll();
	
	CHECK(supply.getFoodCount() == 0);
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

	colorText(); //option to change text color in console window

	bannerAndInput(name, campers, nightsStaying, firesPlanned, ch);
	supply.extrasFunc(ch, MAX_ARRAY);
	calculations(campers, nightsStaying, firesPlanned, fireStarter,
		lbsMarshmallow);
	printSave(supply, MAX_ARRAY, name, campers, nightsStaying,
		firesPlanned, fireStarter, lbsMarshmallow, ch, raiseFlag);

	//if exceptions are raised in printSave function, exit program with error
	if (raiseFlag)
	{
		return 1;
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

	if (!(cin >> campers)) {//initialize campers variable; handle non-numerical entries
		campers = 1; //invalid entries will set variable equal to 1
		cout << "#== The value needs to be an integer! ==#" << endl
			<< "#==== = = - -  I've set the value to 1 =#" << endl << endl;
	}

	cout << endl << "# " << campers << " Campers, okay! Now, how many nights #" << endl
		<< "#======= = - - -   will you be staying? #" << endl << endl;


	cin.clear();          //clear and reset istream in fail state
	cin.ignore(50, '\n');

	if (!(cin >> nightsStaying)) {//initialize nightsStaying variable; handle exceptions
		nightsStaying = 1; //invalid entries will set variable equal to 1
		cout << "#== The value needs to be an integer! ==#" << endl
			<< "#==== = = - -  I've set the value to 1 =#" << endl << endl;
	}

	cout << endl << "# " << nightsStaying << " Nights, okay! Now, how many fires ==#" << endl
		<< "#==== - do you plan to have each day? ==#" << endl;


	cin.clear();          //clear and reset istream in fail state
	cin.ignore(50, '\n');

	if (!(cin >> firesPlanned)) {//initialize firesPlanned variable; handle exceptions
		firesPlanned = 2; //invalid entries will set variable equal to 2
		cout << "#== The value needs to be an integer! ==#" << endl
			<< "#==== = = - -  I've set the value to 2  =#" << endl << endl;
	}

	cout << endl << "# " << firesPlanned << " Campfires/day... Alright, now...  ==#"
		<< endl << "# are there any other items you would ==#" << endl
		<< "# like to add to your camping list?  ===#" << endl
		<< "#==== = = - - - -    Enter Y for yes ===#" << endl << endl;

	cin.clear();          //clear and reset istream in fail state
	cin.ignore(50, '\n');

	cin >> ch; //get user response for adding an extra item
}

void calculations(int campers, int nightsStaying, int firesPlanned, int& fireStarter,
	double& lbsMarshmallow)
{
	calculateFireStarters(firesPlanned, nightsStaying, fireStarter);
	calculateMarshmallows(fireStarter, campers, lbsMarshmallow);
}

int menuSelect()//int menu)
{
	int menu;
	cout << "#---- How would you like to proceed? ---#" << endl << endl
		<< "#----  1 = Make/Replace Itinerary   ----#" << endl
		<< "#----  2 = Save and display on screen  -#" << endl
		<< "#----  3 = Save to file only   ---------#" << endl << endl;
	cin >> menu;//get user menu selection
	return menu;
}

void printSave(SupplyList& supply, int length, string name,
	int campers, int nightsStaying, int firesPlanned, int fireStarter, double lbsMarshmallow, char ch, bool& raiseFlag)

{
	int menu;
	string activity;
	string extraItem;
	bool itinerary = false;
	ofstream outData;
	ifstream inData;
	//raiseFlag = false;

	do {

		menu = menuSelect();

		switch (menu) {
		case 1:
			itFunc(nightsStaying, activity, itinerary);
			break;


		case 2:
			cout << "#=====   Printing list to screen   =====#" << endl << endl << endl;
			if (itinerary) //if itinerary was created, read from 'it' file
			{
				inData.open(it);

				if (!inData) // open input file; handle exceptions
				{
					raiseFlag = true;
					cout << "#== Cannot open file, exiting program ==#" << endl << endl;
					return;
				}

				cout << "#*********************************#" << endl
					<< right << setw(19) << setfill('.') << name    //
					<< left << setw(16) << "'s Itinerary" << endl
					<< right << "#*********************************#" << endl;


				while (inData) // read and print itinerary to screen
				{
					getline(inData, activity);
					cout << activity << endl;
				}
				inData.close();

			}
			cout << "#*********************************#" << endl;
			cout << right << setw(19) << setfill('.') << name    //name the camping list
				<< left << "'s Camp Supplies" << endl
				<< showpoint << fixed << setprecision(2) //will only apply to lbsMarshmallow variable
				<< setw(28) << "Size of camping party:"
				<< right << setw(7) << campers << endl  //set party size = campers variable
				<< left << setw(28) << "Duration of stay:"
				<< right << setw(7) << nightsStaying << endl  //duration of stay = nightsStaying
				<< setfill('-') << "#" << setw(34) << "#" << endl
				<< "#" << setw(34) << "#" << endl
				<< setfill('.') << "#" << setw(16) << "Things "
				<< left << setw(17) << "to pack" << "#" << endl
				<< "Need: " << static_cast<int>((campers / 2.0) + .5)      //want to round up for both 2 man and
				<< "x 2-man OR " << static_cast<int>((campers / 5.0) + .8) //5 man tents thus adding .5 & .8 to
				<< "x 5-man tent(s)" << endl								//static_cast<int>
				<< setw(28) << "- - - Sleeping bags"
				<< right << setw(7) << campers << endl   //set # of sleeping bags = campers variable
				<< left << setw(28) << "- - - Fire starters"
				<< right << setw(7) << fireStarter << endl //print # of fire starters to bring
				<< left << setw(28) << "- - - Lbs of Marshmallow"
				<< right << setw(7) << lbsMarshmallow << endl; //print weight of marshmallows to bring
			if (ch == 'y' || ch == 'Y')
			{
				// use supply instance directly
				supply.printItems(length);
				if (raiseFlag)
				{
					return;
				}

			}
			cout << "#" << setfill('-') << setw(34) << "#" << endl
				<< setfill('.') << setw(26) << "And lots of camping "
				<< left << "spirit!!!" << endl
				<< "#*********************************#"
				<< endl << endl << endl;

		case 3:
			outData.open(rpt);
			cout << "#======    Saving your report now...  ==#" << endl;

			if (itinerary) //if itinerary was created, read from 'it' file
			{

				inData.open(it);

				if (!inData) //open input file; handle exceptions
				{
					raiseFlag = true;
					cout << "#== Cannot open file, exiting program ==#" << endl << endl;
					return;
				}

				outData << "#*********************************#" << endl
					<< setw(19) << setfill('.') << name    //name the itinerary for the output file
					<< left << setw(17) << "'s Itinerary" << endl
					<< right << "#*********************************#" << endl;

				while (inData) // read from itinerary file and write to report file
				{
					getline(inData, activity);
					outData << activity << endl;
				}
				inData.close();

			}
			outData << "#*********************************#" << endl;
			outData << setw(19) << setfill('.') << name    //name the camping list
				<< left << "'s Camp Supplies" << endl
				<< showpoint << fixed << setprecision(2) //will only apply to lbsMarshmallow variable
				<< setw(28) << "Size of camping party:"
				<< right << setw(7) << campers << endl  //set party size = campers variable
				<< left << setw(28) << "Duration of stay:"
				<< right << setw(7) << nightsStaying << endl  //duration of stay = nightsStaying
				<< setfill('-') << "#" << setw(34) << "#" << endl
				<< "#" << setw(34) << "#" << endl
				<< setfill('.') << "#" << setw(16) << "Things "
				<< left << setw(17) << "to pack" << "#" << endl
				<< "Need: " << static_cast<int>((campers / 2.0) + .5)      //want to round up for both 2 man and
				<< "x 2-man OR " << static_cast<int>((campers / 5.0) + .8) //5 man tents thus adding .5 & .8 to
				<< "x 5-man tent(s)" << endl								//static_cast<int>
				<< setw(28) << "- - - Sleeping bags"
				<< right << setw(7) << campers << endl   //set # of sleeping bags = campers variable
				<< left << setw(28) << "- - - Fire starters"
				<< right << setw(7) << fireStarter << endl //print # of fire starters to bring
				<< left << setw(28) << "- - - Lbs of Marshmallow"
				<< right << setw(7) << lbsMarshmallow << endl; //print weight of marshmallows to bring
			if (ch == 'y' || ch == 'Y')
			{
				// supply already contains items added earlier
				supply.addItems(outData, raiseFlag, length);
				if (raiseFlag)
				{
					return;
				}

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
			return; //exit program with error code if invalid menu selection
		}
	} while (menu == 1);

}

void itFunc(int nightsStaying, string activity, bool& itinerary)
{
	ofstream outData;

	itinerary = true; //set itinerary boolean to true if user wants to create itinerary
	outData.open(it);
	cout << endl << "# Let's build an itinerary for the trip #" << endl << endl;
	cin.clear();
	cin.ignore(1, '\n');
	for (int n = 1; n <= nightsStaying; n++) //for loop to get activities for each day
		//for loop requirement for assignment 4
	{
		cout << "#==  What activity do you have planned =#" << endl
			<< "#================ = - -   for day " << n << "? ===#" << endl
			<< "# (You can say things like \"Kayaking\", =#" << endl
			<< "#=========   \"Hiking\", or \"Fishing\"  ===#" << endl;
		getline(cin, activity);
		cout << "<(" << activity << ") added to itinerary>" << endl << endl;
		outData << left << setfill(' ') << "*Day " << right << setw(2) << n
			<< ": " << right << setw(25) << activity << "*" << endl;
	}
	outData.close();
	cout << "# Perfect! I've created your ===========#" << endl
		<< "#=============  personalized itinerary. #" << endl;
	//break;

}

void colorText()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COLOR);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu