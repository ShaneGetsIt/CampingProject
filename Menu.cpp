#include "Menu.h"

// Default constructor
Menu::Menu()
{
	userName = "";
	numOptions = 0;
	menuPriority = medium;

	// Initialize options array
	for (int i = 0; i < MAX_OPTIONS; i++)
	{
		menuOptions[i] = "";
	}
}

// Parameterized constructor
Menu::Menu(string name, int options, priority prior)
{
	userName = name;
	menuPriority = prior;

	// Validate and set number of options
	if (options > MAX_OPTIONS)
	{
		numOptions = MAX_OPTIONS;
	}
	else if (options < 0)
	{
		numOptions = 0;
	}
	else
	{
		numOptions = options;
	}

	// Initialize options array
	for (int i = 0; i < MAX_OPTIONS; i++)
	{
		menuOptions[i] = "";
	}
}

// Setters
void Menu::setUserName(string name)
{
	userName = name;
}

void Menu::setNumOptions(int options)
{
	if (options > MAX_OPTIONS)
	{
		numOptions = MAX_OPTIONS;
	}
	else if (options < 0)
	{
		numOptions = 0;
	}
	else
	{
		numOptions = options;
	}
}

void Menu::setMenuPriority(priority prior)
{
	menuPriority = prior;
}

void Menu::setOption(int index, string optionText)
{
	if (index >= 0 && index < numOptions && index < MAX_OPTIONS)
	{
		menuOptions[index] = optionText;
	}
}

// Getters
string Menu::getUserName() const
{
	return userName;
}

int Menu::getNumOptions() const
{
	return numOptions;
}

priority Menu::getMenuPriority() const
{
	return menuPriority;
}

string Menu::getOption(int index) const
{
	if (index >= 0 && index < numOptions)
	{
		return menuOptions[index];
	}
	return "";
}

string Menu::getPriorityString() const
{
	switch (menuPriority)
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

// Print menu to screen (meets base class requirement)
void Menu::print() const
{
	cout << "#---- How would you like to proceed? ---#" << endl;
	if (!userName.empty())
	{
		cout << "#---- Menu for: " << left << setw(20) << userName << " ----#" << endl;
	}
	cout << endl;

	for (int i = 0; i < numOptions; i++)
	{
		if (!menuOptions[i].empty())
		{
			cout << "#----  " << (i + 1) << " = " << left << setw(30) << menuOptions[i] << " ----#" << endl;
		}
	}

	cout << endl;
}

// Get user's menu selection
int Menu::getSelection()
{
	int choice;
	cin >> choice;
	return choice;
}