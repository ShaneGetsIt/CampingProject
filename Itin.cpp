#include "Itin.h"

// Initialize static member
int Itin::totalItineraries = 0;

// Default constructor
Itin::Itin()
{
	nightsStaying = 0;
	activityCount = 0;
	
	// Initialize arrays to empty/default values
	for (int i = 0; i < MAX_NIGHTS; i++)
	{
		activities[i] = "";
		importance[i] = medium;
	}
}

// Parameterized constructor
Itin::Itin(int nights, priority prior)
{
	if (nights > MAX_NIGHTS)
	{
		nightsStaying = MAX_NIGHTS;
	}
	else if (nights < 0)
	{
		nightsStaying = 0;
	}
	else
	{
		nightsStaying = nights;
	}
	
	activityCount = 0;
	
	// Initialize arrays with default priority
	for (int i = 0; i < MAX_NIGHTS; i++)
	{
		activities[i] = "";
		importance[i] = prior;
	}
}

// Setters
void Itin::setNightsStaying(int nights)
{
	if (nights > MAX_NIGHTS)
	{
		nightsStaying = MAX_NIGHTS;
	}
	else if (nights < 0)
	{
		nightsStaying = 0;
	}
	else
	{
		nightsStaying = nights;
	}
	
	// Reset activity count if new nights is less than current count
	if (activityCount > nightsStaying)
	{
		activityCount = nightsStaying;
	}
}

void Itin::setImportance(int index, priority prior)
{
	if (index >= 0 && index < activityCount)
	{
		importance[index] = prior;
	}
}

void Itin::addActivity(const string& activity, priority prior)
{
	if (activityCount < nightsStaying && activityCount < MAX_NIGHTS)
	{
		activities[activityCount] = activity;
		importance[activityCount] = prior;
		activityCount++;
	}
}

void Itin::addActivity(const string& activity)
{
	addActivity(activity, medium);  // default to medium priority
}

// Getters
int Itin::getNightsStaying() const
{
	return nightsStaying;
}

priority Itin::getImportance(int index) const
{
	if (index >= 0 && index < activityCount)
	{
		return importance[index];
	}
	return medium;  // default return
}

string Itin::getActivity(int index) const
{
	if (index >= 0 && index < activityCount)
	{
		return activities[index];
	}
	return "";
}

int Itin::getActivityCount() const
{
	return activityCount;
}

string Itin::getPriorityString(priority prior) const
{
	switch (prior)
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

string Itin::getPriorityString(int index) const
{
	if (index >= 0 && index < activityCount)
	{
		return getPriorityString(importance[index]);
	}
	return "Unknown";
}

int Itin::getTotalItineraries()
{
	return totalItineraries;
}

// Build itinerary interactively
void Itin::buildItinerary()
{
	cout << endl << "# Let's build an itinerary for the trip #" << endl << endl;
	cin.clear();
	cin.ignore(1, '\n');
	
	for (int n = 0; n < nightsStaying; n++)
	{
		string activity;
		cout << "#==  What activity do you have planned =#" << endl
			<< "#================ = - -   for day " << (n + 1) << "? ===#" << endl
			<< "# (You can say things like \"Kayaking\", =#" << endl
			<< "#=========   \"Hiking\", or \"Fishing\"  ===#" << endl;
		getline(cin, activity);
		addActivity(activity, medium);  // default priority for interactive mode
		cout << "<(" << activity << ") added to itinerary>" << endl << endl;
	}
	
	cout << "# Perfect! I've created your ===========#" << endl
		<< "#=============  personalized itinerary. #" << endl;
}

// Write to specified file
void Itin::writeToFile(const string& filename)
{
	ofstream outData;
	outData.open(filename);
	
	if (!outData)
	{
		cout << "#== Cannot open " << filename << " for writing ==#" << endl;
		return;
	}
	
	for (int n = 0; n < activityCount; n++)
	{
		outData << left << setfill(' ') << "*Day " << right << setw(2) << (n + 1)
			<< ": " << right << setw(25) << activities[n] 
			<< " [" << getPriorityString(n) << "]" << "*" << endl;
	}
	
	outData.close();
}

// Write to indexed file (auto-increments counter with circular wrapping)
void Itin::writeIndexedFile()
{
	totalItineraries++;  // Increment shared counter
	int fileNum = ((totalItineraries - 1) % MAX_ITINERARIES) + 1;  // Wraps 1-10
	string filename = "It_" + to_string(fileNum) + ".txt";
	writeToFile(filename);
}

// Print to screen (meets base class requirement)
void Itin::print() const
{
	cout << "#*********************************#" << endl;
	cout << "#        Itinerary Details        #" << endl;
	cout << "#*********************************#" << endl;
	cout << "Nights Staying: " << nightsStaying << endl;
	cout << "Activities planned: " << activityCount << endl;
	
	for (int i = 0; i < activityCount; i++)
	{
		cout << "  Day " << (i + 1) << ": " << activities[i] 
			<< " [" << getPriorityString(i) << "]" << endl;
	}
	
	cout << "#*********************************#" << endl;
}