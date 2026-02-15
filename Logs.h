#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <fstream>
#include <iostream>
#include "Itin.h"
#include "GeneralSupplies.h"  // CHANGED from SupplyList.h
using namespace std;

const int MAX_LOGS = 10;  // Maximum saved logs (circular buffer like itineraries)

class Logs
{
private:
	string logName;              // Name/title of this camping log
	Itin itinerary;              // Composition: contains an Itin object
	GeneralSupplies supplies;    // CHANGED: Composition: contains a GeneralSupplies object
	static int totalLogs;        // Shared counter across all Logs (persisted)

public:
	// Constructors
	Logs();
	Logs(string name, const Itin& itin, const SupplyList& supply);

	// Setters
	void setLogName(string name);
	void setItinerary(const Itin& itin);
	void setSupplies(const SupplyList& supply);

	// Getters
	string getLogName() const;
	Itin getItinerary() const;
	const SupplyList& getSupplies() const;  // Returns base class reference (slicing is OK here)
	static int getTotalLogs();

	// File persistence for log count
	static void loadLogCount();
	static void saveLogCount();

	// Core functionality
	void saveLog();              // Save this log to indexed file
	void loadLog(int logNumber); // Load a log from file
	void print() const;          // Display log summary
};

#endif