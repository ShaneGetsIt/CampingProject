#ifndef LOGSELECT_H
#define LOGSELECT_H

#include "Menu.h"
#include "Logs.h"
#include <fstream>
using namespace std;

class LogSelect : public Menu
{
private:
	Logs selectedLog;        // The currently selected log
	int selectedLogNumber;   // Which log number was selected

public:
	// Constructors
	LogSelect();
	LogSelect(string name);

	// Setters
	void setSelectedLog(const Logs& log);
	void setSelectedLogNumber(int logNum);

	// Getters
	Logs getSelectedLog() const;
	int getSelectedLogNumber() const;

	// Core functionality (overrides and extends base)
	void buildLogMenu();     // Populate menu with available logs
	void print() const;      // Override: Display log selection menu
	void selectLog();        // Get user selection and load that log
};

#endif