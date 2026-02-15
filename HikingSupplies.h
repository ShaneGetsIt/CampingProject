#ifndef HIKINGSUPPLIES_H
#define HIKINGSUPPLIES_H

#include "SupplyList.h"
#include <string>
using namespace std;

class HikingSupplies : public SupplyList
{
private:
	int trailDifficulty;     // 1=Easy, 2=Moderate, 3=Difficult
	double distanceMiles;    // Total hiking distance

public:
	// Constructors
	HikingSupplies();
	HikingSupplies(string name, int capacity, priority prio, int difficulty, double distance);

	// Virtual destructor
	virtual ~HikingSupplies();

	// Override pure virtual function from base class
	virtual string getSupplyType() const override;

	// Setters
	void setTrailDifficulty(int difficulty);
	void setDistanceMiles(double distance);

	// Getters
	int getTrailDifficulty() const;
	double getDistanceMiles() const;
	string getDifficultyString() const;

	// Override/extend base functionality
	void printItems(int length);           // Override: Add hiking-specific display
	void addHikingEssentials();            // Add recommended hiking items based on difficulty
	void print() const;                    // Display hiking supply summary
};

#endif