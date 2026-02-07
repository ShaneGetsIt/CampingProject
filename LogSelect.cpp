#include "LogSelect.h"

// Default constructor
LogSelect::LogSelect() : Menu()
{
	selectedLogNumber = 0;
}

// Parameterized constructor
LogSelect::LogSelect(string name) : Menu(name, 0, medium)
{
	selectedLogNumber = 0;
	buildLogMenu();  // Automatically build menu on construction
}

// Setters
void LogSelect::setSelectedLog(const Logs& log)
{
	selectedLog = log;
}

void LogSelect::setSelectedLogNumber(int logNum)
{
	selectedLogNumber = logNum;
}

// Getters
Logs LogSelect::getSelectedLog() const
{
	return selectedLog;
}

int LogSelect::getSelectedLogNumber() const
{
	return selectedLogNumber;
}

// Build menu with available logs
void LogSelect::buildLogMenu()
{
	int totalLogs = Logs::getTotalLogs();
	
	// Set number of options based on available logs
	if (totalLogs > MAX_LOGS)
	{
		setNumOptions(MAX_LOGS);
	}
	else if (totalLogs > 0)
	{
		setNumOptions(totalLogs);
	}
	else
	{
		setNumOptions(0);
		return;
	}

	// Read log names from files and populate menu options
	for (int i = 0; i < numOptions; i++)
	{
		int logNum = i + 1;
		string filename = "Log_" + to_string(logNum) + ".txt";
		
		ifstream inFile(filename);
		if (inFile)
		{
			string line;
			// Skip first line (header)
			getline(inFile, line);
			// Get second line with log name
			getline(inFile, line);
			
			// Extract log name from line
			size_t pos = line.find("Log: ");
			string logName = "Log " + to_string(logNum);
			
			if (pos != string::npos)
			{
				logName = line.substr(pos + 5);
				// Remove any trailing characters
				size_t endPos = logName.find_first_of("\r\n");
				if (endPos != string::npos)
				{
					logName = logName.substr(0, endPos);
				}
			}
			
			setOption(i, logName);
			inFile.close();
		}
		else
		{
			// File doesn't exist
			setOption(i, "Log " + to_string(logNum) + " (empty)");
		}
	}
}

// Print menu (overrides base class print)
void LogSelect::print() const
{
	cout << "#---- Select a Camping Log to View ----#" << endl;
	
	if (!userName.empty())
	{
		cout << "#---- " << left << setw(30) << userName << " ----#" << endl;
	}
	
	if (numOptions == 0)
	{
		cout << "#---- No logs available yet ----#" << endl;
		return;
	}
	
	cout << endl;

	for (int i = 0; i < numOptions; i++)
	{
		string option = getOption(i);
		if (!option.empty())
		{
			cout << "#----  " << (i + 1) << " = " << left << setw(30) << option << " ----#" << endl;
		}
	}

	cout << "#----  0 = Cancel/Go Back           ----#" << endl;
	cout << endl;
}

// Select and load a log
void LogSelect::selectLog()
{
	// First, rebuild menu in case logs were added since construction
	buildLogMenu();
	
	// Display menu
	print();
	
	if (numOptions == 0)
	{
		cout << "#== No logs to select ==#" << endl;
		return;
	}
	
	// Get user selection
	cout << "Enter selection: ";
	int choice = getSelection();
	
	// Validate choice
	if (choice == 0)
	{
		cout << "#== Selection cancelled ==#" << endl;
		return;
	}
	
	if (choice < 1 || choice > numOptions)
	{
		cout << "#== Invalid selection ==#" << endl;
		return;
	}
	
	// Load and display the selected log
	selectedLogNumber = choice;
	string filename = "Log_" + to_string(selectedLogNumber) + ".txt";
	
	ifstream inFile(filename);
	if (!inFile)
	{
		cout << "#== Log file " << filename << " not found ==#" << endl;
		return;
	}
	
	// Display the entire log file contents
	cout << endl << "#=== Displaying Log " << selectedLogNumber << " ===#" << endl << endl;
	
	string line;
	while (getline(inFile, line))
	{
		cout << line << endl;
	}
	
	inFile.close();
	
	cout << endl << "#=== End of Log ===#" << endl;
}