#ifndef MENU_H
#define MENU_H

#include <string>
#include <iostream>
#include <iomanip>
#include "SupplyList.h"  // For priority enum
using namespace std;

const int MAX_OPTIONS = 10;  // Maximum menu options

class Menu
{
protected:
	string userName;              // User's name (protected for derived classes)
	int numOptions;               // Number of menu options (protected)

private:
	string menuOptions[MAX_OPTIONS];  // Array of option strings
	priority menuPriority;            // Menu importance/urgency

public:
	// Constructors
	Menu();
	Menu(string name, int options, priority prior);

	// Setters
	void setUserName(string name);
	void setNumOptions(int options);
	void setMenuPriority(priority prior);
	void setOption(int index, string optionText);

	// Getters
	string getUserName() const;
	int getNumOptions() const;
	priority getMenuPriority() const;
	string getOption(int index) const;
	string getPriorityString() const;

	// Core functionality
	void print() const;                // Display menu (base class requirement)
	int getSelection();                // Get user's menu choice
};

#endif