![Tests](https://github.com/ShaneGetsIt/CampingProject/actions/workflows/tests.yml/badge.svg)

# C++ Programming Project
This is the C++ programming project for ITCS-2530.
The program is a camping-packing assistant.
The program takes several user inputs regarding the user's upcoming trip.
The program then outputs the supplies they should bring based on the user's input. 
The program produces a menu which asks user to add an itinerary,
save and print to screen, or save only.


The following instructions are in the order you can expect them to be relevant:

For your name, the program will receive up to a 25 character string as input.
The subsequent 3 user inputs ask for an integer value, but will only display
up to 2 digits - please bear this in mind.

If you wish to add extra items to the supply list, enter Y or y when prompted.
You will be asked to enter '1' for food, '2' for gear and '3' when finished.
The extra items prompt takes string inputs, but will only print out to 21 characters.
You must enter each additional item one at a time; when you are done entering
additional items, enter "3".

Once the user's information has been collected, the menu will be displayed;
User will be asked to either create(an)/replace(the existing) itinerary,
save the report AND display it on the console or save the file only.
Simply enter 1,2 or 3. Thanks for reading!

## Class Diagram

    class SupplyList {
        #string listName
        #int maxCapacity
        #priority listPriority
        -inventoryItem foodItems[]
        -inventoryItem gearItems[]
        -int foodCount
        -int gearCount
        +SupplyList()
        +SupplyList(string, int, priority)
        +addFoodItem(inventoryItem) bool
        +addGearItem(inventoryItem) bool
        +getFoodCount() int
        +getGearCount() int
        +getTotalItemCount() int
        +getAverageFoodQuantity() double
        +printItems(int) void
        +clearAll() void
        +print() void
    }

    class HikingSupplies {
        -int trailDifficulty
        -double distanceMiles
        +HikingSupplies()
        +HikingSupplies(string, int, priority, int, double)
        +setTrailDifficulty(int) void
        +setDistanceMiles(double) void
        +getTrailDifficulty() int
        +getDistanceMiles() double
        +getDifficultyString() string
        +addHikingEssentials() void
        +printItems(int) void
        +print() void
    }

    class Itin {
        #int nightsStaying
        -string activities[]
        -priority importance[]
        -int activityCount
        -static int totalItineraries
        +Itin()
        +Itin(int, priority)
        +setNightsStaying(int) void
        +addActivity(string, priority) void
        +getNightsStaying() int
        +getActivityCount() int
        +getActivity(int) string
        +buildItinerary() void
        +writeToFile(string) void
        +writeIndexedFile() void
        +print() void
    }

    class Menu {
        #string userName
        #int numOptions
        -string menuOptions[]
        -priority menuPriority
        +Menu()
        +Menu(string, int, priority)
        +setUserName(string) void
        +setNumOptions(int) void
        +setOption(int, string) void
        +getUserName() string
        +getNumOptions() int
        +getMenuPriority() priority
        +getOption(int) string
        +print() void
        +getSelection() int
    }

    class LogSelect {
        -Logs selectedLog
        -int selectedLogNumber
        +LogSelect()
        +LogSelect(string)
        +setSelectedLog(Logs) void
        +setSelectedLogNumber(int) void
        +getSelectedLog() Logs
        +getSelectedLogNumber() int
        +buildLogMenu() void
        +print() void
        +selectLog() void
    }

    class Logs {
        -string logName
        -Itin itinerary
        -SupplyList supplies
        -static int totalLogs
        +Logs()
        +Logs(string, Itin, SupplyList)
        +setLogName(string) void
        +getLogName() string
        +getItinerary() Itin
        +getSupplies() SupplyList
        +static getTotalLogs() int
        +static loadLogCount() void
        +static saveLogCount() void
        +saveLog() void
        +loadLog(int) void
        +print() void
    }

    %% Inheritance relationships
    SupplyList <|-- HikingSupplies : inherits
    Menu <|-- LogSelect : inherits

    %% Composition relationships
    Logs *-- Itin : contains
    Logs *-- SupplyList : contains
    LogSelect o-- Logs : uses

    %% Notes
    note for SupplyList "Base class for supply management\nUses enum priority and struct inventoryItem"
    note for HikingSupplies "Derived class: adds trail-specific features\nAuto-populates items based on difficulty"
    note for Itin "Manages trip itinerary\nStatic counter with file persistence"
    note for Menu "Base class for menu systems\nFlexible option management"
    note for LogSelect "Derived class: displays saved logs\nReads from persisted log files"
    note for Logs "Composition class\nAggregates Itin + SupplyList\nPersists trip data to files"
