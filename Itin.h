#ifndef ITIN_H
#define ITIN_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "SupplyList.h"  // For priority enum
using namespace std;

const int MAX_NIGHTS = 10;       // Maximum nights for itinerary
const int MAX_ITINERARIES = 10;  // Maximum saved itinerary files (then wraps)

class Itin
{
protected:
	int nightsStaying;  // protected so derived classes can access

private:
	string activities[MAX_NIGHTS];    // fixed array of activity names
	priority importance[MAX_NIGHTS];  // priority for each activity (using global enum)
	int activityCount;                // tracks how many activities added
	static int totalItineraries;      // shared counter across all Itin objects
	
public:
	// Constructors
	Itin();
	Itin(int nights, priority prior);
	
	// Setters
	void setNightsStaying(int nights);
	void setImportance(int index, priority prior);
	void addActivity(const string& activity, priority prior);
	void addActivity(const string& activity);  // uses medium as default
	
	// Getters
	int getNightsStaying() const;
	priority getImportance(int index) const;
	string getActivity(int index) const;
	int getActivityCount() const;
	string getPriorityString(priority prior) const;
	string getPriorityString(int index) const;
	static int getTotalItineraries();  // get counter value
	
	// Core functionality
	void buildItinerary();
	void writeToFile(const string& filename);
	void writeIndexedFile();  // Auto-increments and uses next file number
	void print() const;
};

#endif