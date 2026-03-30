#ifndef ITIN_H
#define ITIN_H

#include <string>
#include <fstream>
#include "Stack.h"  // WEEK 11: Stack for undo/redo functionality
#include "SupplyList.h"

using namespace std;

// Priority levels for activities
//enum priority { low, medium, high };

class Itin
{
public:
    Itin();
    Itin(int nights, priority p);
    
    // Activity management
    void addActivity(string activity, priority p);
    void buildItinerary();
    
    // Undo/Redo functionality using Stack
    bool undoLastActivity();
    bool redoLastActivity();
    bool canUndo() const;
    bool canRedo() const;
    
    // File operations
    void writeToFile(string filename);
    void writeIndexedFile();
    
    // Getters
    int getNightsStaying() const;
    int getActivityCount() const;
    string getActivity(int index) const;
    priority getActivityPriority(int index) const;
    
    // Display
    void printItinerary() const;
    void printUndoRedoStatus() const;
    
private:
    struct Activity
    {
        string name;
        priority priorityLevel;
    };
    
    Activity* activities;        // Dynamic array of activities
    int nightsStaying;
    int activityCount;
    int maxActivities;
    priority itineraryPriority;
    
    // Undo/Redo stacks - store activity indices
    Stack undoStack;    // Stores count of activities before each add
    Stack redoStack;    // Stores activities that were undone
    
    // Helper functions
    void resizeActivities();
    void saveStateForUndo();
    string getPriorityString(priority p) const;
};

#endif