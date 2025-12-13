#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

class SupplyList
{
public:
	enum itemType { food, gear };
	enum priority { low, medium, high };

	struct inventoryItem
	{
		string itemName;
		itemType item = food;
		priority prio = low;
		int quantity = 0;
	};

	static const int MAX_ARRAY = 5;

	void bannerAndInput(string& name, int& campers, int& nightsStaying, int& fireStarter, char& ch);
	void extrasFunc(char ch, inventoryItem foodList[], inventoryItem gearList[], int& foodCount, int& gearCount, int length);
	void calculations(int campers, int nightsStaying, int firesPlanned, int& fireStarter, double& lbsMarshmallow);
	void printSave(int foodCount, int gearCount, const inventoryItem foodList[], const inventoryItem gearList[], int length, string name,
		int campers, int nightsStaying, int firesPlanned, int fireStarter, double lbsMarshmallow, char ch, bool& raiseFlag);
	void colorText();
	SupplyList() {} // default constructor

private:
	void printItems(const inventoryItem foodList[], const inventoryItem gearList[], int foodCount, int gearCount, int length) const;
	void addItems(const inventoryItem foodList[], const inventoryItem gearList[], int foodCount, int gearCount, int length,
		ofstream& outData, bool& raiseFlag) const;
	void itFunc(int nightsStaying, string name, bool& itinerary);
	void menuSelect(int& menu);

	// file names and console color
	const string extras = "Extras.txt";
	const string it = "It.txt";
	const string rpt = "Report.txt";
	const int COLOR = 100;
};