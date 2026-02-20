#include "HikingSupplies.h"

// Default constructor
HikingSupplies::HikingSupplies() : SupplyList()
{
	trailDifficulty = 1;  // Easy by default
	distanceMiles = 0.0;
}

// Parameterized constructor
HikingSupplies::HikingSupplies(string name, int capacity, priority prio, int difficulty, double distance)
	: SupplyList(name, capacity, prio)
{
	// Validate difficulty
	if (difficulty < 1)
	{
		trailDifficulty = 1;
	}
	else if (difficulty > 3)
	{
		trailDifficulty = 3;
	}
	else
	{
		trailDifficulty = difficulty;
	}

	// Validate distance
	if (distance < 0)
	{
		distanceMiles = 0.0;
	}
	else
	{
		distanceMiles = distance;
	}
}

// Setters
void HikingSupplies::setTrailDifficulty(int difficulty)
{
	if (difficulty < 1)
	{
		trailDifficulty = 1;
	}
	else if (difficulty > 3)
	{
		trailDifficulty = 3;
	}
	else
	{
		trailDifficulty = difficulty;
	}
}

void HikingSupplies::setDistanceMiles(double distance)
{
	if (distance < 0)
	{
		distanceMiles = 0.0;
	}
	else
	{
		distanceMiles = distance;
	}
}

// Getters
int HikingSupplies::getTrailDifficulty() const
{
	return trailDifficulty;
}

double HikingSupplies::getDistanceMiles() const
{
	return distanceMiles;
}

string HikingSupplies::getDifficultyString() const
{
	switch (trailDifficulty)
	{
	case 1:
		return "Easy";
	case 2:
		return "Moderate";
	case 3:
		return "Difficult";
	default:
		return "Unknown";
	}
}

string HikingSupplies::getSupplyType() const
{
	return "Hiking Supplies - " + getDifficultyString() + " Trail";
}

// WEEK 06 ADDITION: Virtual function for polymorphic streaming
// Called by operator<< in base class
void HikingSupplies::toStream(ostream& out) const
{
	// Use 'this' pointer explicitly to demonstrate requirement
	out << "HikingSupplies[" << this->getListName()
		<< ", " << this->getDifficultyString() << " Trail"
		<< ", " << this->getDistanceMiles() << " miles"
		<< ", Priority: " << this->getPriorityString(this->getListPriority())
		<< ", Items: " << this->getTotalItemCount() << "]";
}

// Override printItems to add hiking-specific information
void HikingSupplies::printItems(int length)
{
	cout << endl << "#=== Hiking-Specific Supply List ===#" << endl;
	cout << "Trail Difficulty: " << getDifficultyString() << endl;
	cout << "Distance: " << fixed << setprecision(1) << distanceMiles << " miles" << endl;
	cout << endl;

	// Call base class printItems
	SupplyList::printItems(length);
}

// Add recommended hiking items based on difficulty
void HikingSupplies::addHikingEssentials()
{
	inventoryItem item;

	// Essential items for all hikes
	item = { "Water Bottles", food, high, 2 };
	addFoodItem(item);

	item = { "Trail Map", gear, high, 1 };
	addGearItem(item);

	item = { "First Aid Kit", gear, high, 1 };
	addGearItem(item);

	// Additional items for moderate/difficult trails
	if (trailDifficulty >= 2)
	{
		item = { "Energy Bars", food, medium, 5 };
		addFoodItem(item);

		item = { "Hiking Poles", gear, medium, 2 };
		addGearItem(item);
	}

	// Extra items for difficult trails
	if (trailDifficulty == 3)
	{
		item = { "Emergency Shelter", gear, high, 1 };
		addGearItem(item);

		item = { "GPS Device", gear, high, 1 };
		addGearItem(item);
	}

	cout << "#== Hiking essentials added for " << getDifficultyString() << " trail ==#" << endl;
}

// Print hiking supply summary
void HikingSupplies::print() const
{
	cout << "#*********************************#" << endl;
	cout << "#   Hiking Supplies Summary       #" << endl;
	cout << "#*********************************#" << endl;
	cout << "List Name: " << getListName() << endl;
	cout << "Trail Difficulty: " << getDifficultyString() << endl;
	cout << "Distance: " << fixed << setprecision(1) << distanceMiles << " miles" << endl;
	cout << "Priority Level: " << getPriorityString(getListPriority()) << endl;
	cout << "Total Food Items: " << getFoodCount() << endl;
	cout << "Total Gear Items: " << getGearCount() << endl;
	cout << "#*********************************#" << endl;
}

// Destructor
HikingSupplies::~HikingSupplies()
{
	// No additional cleanup needed beyond base class
}