//***********************************************************
// Author: D.S. Malik Style
// Implementation of Itin class with Stack-based undo/redo
//***********************************************************

#include "Itin.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// Default constructor
Itin::Itin()
{
    nightsStaying = 0;
    activityCount = 0;
    maxActivities = 10;
    itineraryPriority = medium;
    activities = new Activity[maxActivities];
}

// Parameterized constructor
Itin::Itin(int nights, priority p)
{
    nightsStaying = nights;
    activityCount = 0;
    maxActivities = 10;
    itineraryPriority = p;
    activities = new Activity[maxActivities];
}

// Add activity with undo support
void Itin::addActivity(string activity, priority p)
{
    // Save current state for undo
    saveStateForUndo();
    
    // Check if resize needed
    if (activityCount >= maxActivities)
    {
        resizeActivities();
    }
    
    // Add new activity
    activities[activityCount].name = activity;
    activities[activityCount].priorityLevel = p;
    activityCount++;
    
    // Clear redo stack when new action is performed
    redoStack.initializeStack();  
}

// Save current state before modification
void Itin::saveStateForUndo()
{
    // Push current activity count onto undo stack
    undoStack.push(activityCount);
}

// Undo last activity addition
bool Itin::undoLastActivity()
{
    if (!canUndo())
    {
        cout << "#== No activities to undo ==#" << endl;
        return false;
    }
    
    // Save current state to redo stack
    redoStack.push(activityCount);
    
    // Get previous state from undo stack
    int previousCount = undoStack.peek();
    undoStack.pop();
    
    // Remove activities added since last state
    while (activityCount > previousCount)
    {
        activityCount--;
    }
    
    cout << "#== Activity undone successfully ==#" << endl;
    return true;
}

// Redo last undone activity
bool Itin::redoLastActivity()
{
    if (!canRedo())
    {
        cout << "#== No activities to redo ==#" << endl;
        return false;
    }
    
    // Get state from redo stack
    int redoCount = redoStack.peek();
    redoStack.pop();
    
    // Push current state to undo stack
    undoStack.push(activityCount);
    
    // Restore to redo state
    activityCount = redoCount;
    
    cout << "#== Activity redone successfully ==#" << endl;
    return true;
}

// Check if undo is possible
bool Itin::canUndo() const
{
    return !undoStack.isEmptyStack();
}

// Check if redo is possible
bool Itin::canRedo() const
{
    return !redoStack.isEmptyStack();
}

// Build itinerary interactively with undo/redo
void Itin::buildItinerary()
{
    string activity;
    char addMore;
    char choice;
    
    cout << endl << "#== Building Itinerary for " << nightsStaying 
         << " nights ==#" << endl;
    cout << "#== Commands: A=Add, U=Undo, R=Redo, D=Done ==#" << endl << endl;
    
    do
    {
        cout << "#== Current activities: " << activityCount << " ==#" << endl;
        printItinerary();
        printUndoRedoStatus();
        
        cout << endl << "# Enter command (A/U/R/D): ";
        cin >> choice;
        cin.ignore(50, '\n');
        
        if (choice == 'A' || choice == 'a')
        {
            cout << "# Enter activity: ";
            getline(cin, activity);
            
            char priorityChoice;
            cout << "# Priority (L=Low, M=Medium, H=High): ";
            cin >> priorityChoice;
            cin.ignore(50, '\n');
            
            priority p = medium;
            if (priorityChoice == 'L' || priorityChoice == 'l')
                p = low;
            else if (priorityChoice == 'H' || priorityChoice == 'h')
                p = high;
            
            addActivity(activity, p);
            cout << "#== Activity added! ==#" << endl;
        }
        else if (choice == 'U' || choice == 'u')
        {
            undoLastActivity();
        }
        else if (choice == 'R' || choice == 'r')
        {
            redoLastActivity();
        }
        else if (choice == 'D' || choice == 'd')
        {
            break;
        }
        else
        {
            cout << "#== Invalid command ==#" << endl;
        }
        
        cout << endl;
        
    } while (true);
    
    cout << endl << "#== Itinerary complete! ==#" << endl;
}

// Print current itinerary
void Itin::printItinerary() const
{
    if (activityCount == 0)
    {
        cout << "  (No activities yet)" << endl;
        return;
    }
    
    for (int i = 0; i < activityCount; i++)
    {
        cout << "  " << (i + 1) << ". " << activities[i].name
             << " [" << getPriorityString(activities[i].priorityLevel) << "]" << endl;
    }
}

// Print undo/redo status
void Itin::printUndoRedoStatus() const
{
    cout << "# Undo available: " << (canUndo() ? "Yes" : "No") << " | ";
    cout << "Redo available: " << (canRedo() ? "Yes" : "No") << " #" << endl;
}

// Write to file
void Itin::writeToFile(string filename)
{
    ofstream outFile;
    outFile.open(filename);
    
    if (!outFile)
    {
        cout << "#== Cannot open file for writing ==#" << endl;
        return;
    }
    
    outFile << "Itinerary for " << nightsStaying << " night camping trip" << endl;
    outFile << "Total Activities: " << activityCount << endl << endl;
    
    for (int i = 0; i < activityCount; i++)
    {
        outFile << "Day " << (i + 1) << ": " << activities[i].name
                << " [" << getPriorityString(activities[i].priorityLevel) << "]" << endl;
    }
    
    outFile.close();
}

// Write indexed file (wraps at 10)
void Itin::writeIndexedFile()
{
    static int fileIndex = 0;
    
    string filename = "Itinerary_" + to_string(fileIndex) + ".txt";
    writeToFile(filename);
    
    fileIndex = (fileIndex + 1) % 10;  // Wrap at 10
}

// Resize activities array
void Itin::resizeActivities()
{
    maxActivities *= 2;
    Activity* newActivities = new Activity[maxActivities];
    
    for (int i = 0; i < activityCount; i++)
    {
        newActivities[i] = activities[i];
    }
    
    delete[] activities;
    activities = newActivities;
}

// Getters
int Itin::getNightsStaying() const
{
    return nightsStaying;
}

int Itin::getActivityCount() const
{
    return activityCount;
}

string Itin::getActivity(int index) const
{
    if (index >= 0 && index < activityCount)
    {
        return activities[index].name;
    }
    return "";
}

priority Itin::getActivityPriority(int index) const
{
    if (index >= 0 && index < activityCount)
    {
        return activities[index].priorityLevel;
    }
    return low;
}

// Helper: Convert priority to string
string Itin::getPriorityString(priority p) const
{
    switch (p)
    {
        case low: return "Low";
        case medium: return "Med";
        case high: return "High";
        default: return "Unknown";
    }
}