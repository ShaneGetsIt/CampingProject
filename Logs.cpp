#include "Logs.h"

// Initialize static member
int Logs::totalLogs = 0;

// Default constructor
Logs::Logs()
{
	logName = "";
	// itinerary and supplies use their default constructors
}

// Parameterized constructor
Logs::Logs(string name, const Itin& itin, const SupplyList& supply)
{
	logName = name;
	itinerary = itin;
	supplies.copyFrom(supply);  // CHANGED: Use copyFrom method
}

// Setters
void Logs::setLogName(string name)
{
	logName = name;
}

void Logs::setItinerary(const Itin& itin)
{
	itinerary = itin;
}

void Logs::setSupplies(const SupplyList& supply)
{
	supplies.copyFrom(supply);  // CHANGED: Use copyFrom method
}

// Getters
string Logs::getLogName() const
{
	return logName;
}

Itin Logs::getItinerary() const
{
	return itinerary;
}

// Return base class reference (slicing is acceptable here since we only need SupplyList interface)
// important***
const SupplyList& Logs::getSupplies() const
{
	return supplies;
}

int Logs::getTotalLogs()
{
	return totalLogs;
}

// Load log count from file (call at program start)
void Logs::loadLogCount()
{
	ifstream inFile("LogCount.dat");
	if (inFile)
	{
		inFile >> totalLogs;
		inFile.close();
	}
	else
	{
		// File doesn't exist, start fresh
		totalLogs = 0;
	}
}

// Save log count to file (call when count changes)
void Logs::saveLogCount()
{
	ofstream outFile("LogCount.dat");
	if (outFile)
	{
		outFile << totalLogs;
		outFile.close();
	}
}

// Save this log to indexed file
void Logs::saveLog()
{
	totalLogs++;
	int fileNum = ((totalLogs - 1) % MAX_LOGS) + 1;  // Wraps 1-10
	string filename = "Log_" + to_string(fileNum) + ".txt";

	ofstream outFile(filename);
	if (!outFile)
	{
		cout << "#== Cannot open " << filename << " for writing ==#" << endl;
		return;
	}

	// Write log header
	outFile << "#*********************************#" << endl;
	outFile << "#     Camping Log: " << logName << endl;
	outFile << "#*********************************#" << endl << endl;

	// Write itinerary summary
	outFile << "Nights: " << itinerary.getNightsStaying() << endl;
	outFile << "Activities: " << itinerary.getActivityCount() << endl << endl;

	// Write supply summary
	outFile << "Food items: " << supplies.getFoodCount() << endl;
	outFile << "Gear items: " << supplies.getGearCount() << endl;

	outFile.close();

	// Save updated count to file
	saveLogCount();

	cout << "#== Log saved as " << filename << " ==#" << endl;
}

// Load a log from file (placeholder - would need more complex file format)
void Logs::loadLog(int logNumber)
{
	if (logNumber < 1 || logNumber > MAX_LOGS)
	{
		cout << "#== Invalid log number ==#" << endl;
		return;
	}

	string filename = "Log_" + to_string(logNumber) + ".txt";
	ifstream inFile(filename);

	if (!inFile)
	{
		cout << "#== Log file " << filename << " not found ==#" << endl;
		return;
	}

	// Read log name from file
	string line;
	getline(inFile, line);  // Skip header line
	getline(inFile, line);  // Get log name line
	
	// Simple parsing - in real implementation would be more robust
	size_t pos = line.find("Log: ");
	if (pos != string::npos)
	{
		logName = line.substr(pos + 5);
	}

	inFile.close();
	cout << "#== Loaded log: " << logName << " ==#" << endl;
}

// Print log summary to screen
void Logs::print() const
{
	cout << "#*********************************#" << endl;
	cout << "#     Camping Log Summary         #" << endl;
	cout << "#*********************************#" << endl;
	cout << "Log Name: " << logName << endl;
	cout << "Total Nights: " << itinerary.getNightsStaying() << endl;
	cout << "Total Activities: " << itinerary.getActivityCount() << endl;
	cout << "Food Items: " << supplies.getFoodCount() << endl;
	cout << "Gear Items: " << supplies.getGearCount() << endl;
	cout << "#*********************************#" << endl;
}