#include "SupplyList.h"

// Read user inputs: name, campers, nights, fires-per-day and extras flag
void SupplyList::bannerAndInput(string& name, int& campers, int& nightsStaying, int& fireStarter, char& ch)
{
    cout << "#=======================================#" << endl
         << "#===== Welcome! You are now using ======#" << endl
         << "# Shane's Camping Supply List Generator #" << endl
         << "# Allow me to take some information to  #" << endl
         << "#  help plan your trip - to begin, what #" << endl
         << "#======================= is your name ? #" << endl << endl;

    getline(cin, name);

    cout << endl << endl << "#== Okay, " << left << setfill('=')
         << setw(25) << name + "  " << "=====#" << endl
         << "#== and how many campers will be going, #" << endl
         << "#=================  including yourself? #" << endl << endl;

    if (!(cin >> campers)) {
        campers = 1;
        cout << "#== The value needs to be an integer! ==#" << endl
             << "#==== = = - -  I've set the value to 1 =#" << endl << endl;
    }

    cout << endl << "# " << campers << " Campers, okay! Now, how many nights #" << endl
         << "#======= = - - -   will you be staying? #" << endl << endl;

    cin.clear();
    cin.ignore(50, '\n');

    if (!(cin >> nightsStaying)) {
        nightsStaying = 1;
        cout << "#== The value needs to be an integer! ==#" << endl
             << "#==== = = - -  I've set the value to 1 =#" << endl << endl;
    }

    cout << endl << "# " << nightsStaying << " Nights, okay! Now, how many fires ==#" << endl
         << "#==== - do you plan to have each day? ==#" << endl;

    cin.clear();
    cin.ignore(50, '\n');

    if (!(cin >> fireStarter)) { // user input is fires per day
        fireStarter = 2;
        cout << "#== The value needs to be an integer! ==#" << endl
             << "#==== = = - -  I've set the value to 2  =#" << endl << endl;
    }

    cout << endl << "# " << fireStarter << " Campfires/day... Alright, now...  ==#" << endl
         << "# are there any other items you would ==#" << endl
         << "# like to add to your camping list?  ===#" << endl
         << "#==== = = - - - -    Enter Y for yes ===#" << endl << endl;

    cin.clear();
    cin.ignore(50, '\n');

    cin >> ch;
}

// extrasFunc fills arrays provided by caller (main). length is the array capacity.
void SupplyList::extrasFunc(char ch, inventoryItem foodList[], inventoryItem gearList[], int& foodCount, int& gearCount, int length)
{
    int choice = 0;

    if (ch == 'y' || ch == 'Y') {
        cin.ignore(1, '\n');
        cout << "# You may add up to " << length << " food items and/or #" << endl
             << "#===== - - - -            " << length << " gear items =#" << endl << endl;

        while (true)
        {
            cout << endl << "#== Please enter '1' to add food, '2' ==#" << endl
                 << "#=====  to add gear or '3' to stop  ====#" << endl << endl;
            cin >> choice;

            while (!choice || choice > 3 || choice < 1)
            {
                cout << "Invalid selection - must enter int's 1-3" << endl;
                cin.clear();
                cin.ignore(12, '\n');
                cin >> choice;
            }

            switch (static_cast<itemType>(choice - 1))
            {
            case food:
                if (foodCount < length)
                {
                    cin.ignore(50, '\n');
                    cout << "#= Enter food item name to add to list =#" << endl << endl;
                    getline(cin, foodList[foodCount].itemName);
                    foodCount++;
                }
                else
                    cout << " Limit for additional food items reached" << endl << endl;
                break;

            case gear:
                if (gearCount < length)
                {
                    cin.ignore(50, '\n');
                    cout << "#= Enter gear item name to add to list =#" << endl
                         << "# for example: kayak, fishing rod, etc. #" << endl << endl;
                    getline(cin, gearList[gearCount].itemName);
                    gearCount++;
                }
                else
                    cout << " Limit for additional gear items reached" << endl << endl;
                break;

            default:
                cout << "#== Finished adding additional items  ==#" << endl << endl;
                return;
            }
        }
    }
    else
        cout << "#===   No extra items to add - got it! =#" << endl << endl;
}

// Write extra items to output file. Use passed stream if open; otherwise open rpt and write there.
void SupplyList::addItems(const inventoryItem foodList[], const inventoryItem gearList[], int foodCount, int gearCount, int length,
    ofstream& outData, bool& raiseFlag) const
{
    // If caller provided an open stream, write to it.
    if (outData.is_open() && outData.good())
    {
        if (foodCount != 0)
        {
            outData << right << setw(35) << setfill('-') << "-" << endl
                    << setw(35) << setfill('.') << "Extra food to pack" << endl;
            for (int i = 0; i < foodCount; ++i)
                outData << setw(35) << foodList[i].itemName << endl;
        }

        if (gearCount != 0)
        {
            outData << right << setw(35) << setfill('-') << "-" << endl
                    << setw(35) << setfill('.') << "Extra gear to pack" << endl;
            for (int i = 0; i < gearCount; ++i)
                outData << setw(35) << gearList[i].itemName << endl;
        }

        return;
    }

    // Otherwise open report file and write there.
    ofstream localOut(rpt);
    if (!localOut)
    {
        cout << "#=====   Cannot open report file for writing   =====#" << endl << endl;
        raiseFlag = true;
        return;
    }

    if (foodCount != 0)
    {
        localOut << right << setw(35) << setfill('-') << "-" << endl
                 << setw(35) << setfill('.') << "Extra food to pack" << endl;
        for (int i = 0; i < foodCount; ++i)
            localOut << setw(35) << foodList[i].itemName << endl;
    }

    if (gearCount != 0)
    {
        localOut << right << setw(35) << setfill('-') << "-" << endl
                 << setw(35) << setfill('.') << "Extra gear to pack" << endl;
        for (int i = 0; i < gearCount; ++i)
            localOut << setw(35) << gearList[i].itemName << endl;
    }

    // close local stream so content is written
    localOut.close();
}

// Print extra items to console
void SupplyList::printItems(const inventoryItem foodList[], const inventoryItem gearList[], int foodCount, int gearCount, int length) const
{
    if (foodCount != 0)
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
             << setw(35) << setfill('.') << "Extra food to pack" << endl;
        for (int i = 0; i < foodCount; ++i)
            cout << setw(35) << foodList[i].itemName << endl;
    }

    if (gearCount != 0)
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
             << setw(35) << setfill('.') << "Extra gear to pack" << endl;
        for (int i = 0; i < gearCount; ++i)
            cout << setw(35) << gearList[i].itemName << endl;
    }
}

void SupplyList::calculations(int campers, int nightsStaying, int firesPlanned, int& fireStarter, double& lbsMarshmallow)
{
    fireStarter = firesPlanned * nightsStaying;
    lbsMarshmallow = (fireStarter * campers) / 4.0;
}

void SupplyList::menuSelect(int& menu)
{
    cout << "#---- How would you like to proceed? ---#" << endl << endl
         << "#----  1 = Make/Replace Itinerary   ----#" << endl
         << "#----  2 = Save and display on screen  -#" << endl
         << "#----  3 = Save to file only   ---------#" << endl << endl;
    cin >> menu;
}

// Print or save the full report; expects arrays/counts passed from main
void SupplyList::printSave(int foodCount, int gearCount, const inventoryItem foodList[], const inventoryItem gearList[], int length, string name,
    int campers, int nightsStaying, int firesPlanned, int fireStarter, double lbsMarshmallow, char ch, bool& raiseFlag)
{
    int menu = 0;
    string activity;
    ofstream outData;
    ifstream inData;
    bool itinerary = false;

    do {
        menuSelect(menu);

        switch (menu) {
        case 1:
            itFunc(nightsStaying, name, itinerary);
            break;

        case 2:
            cout << "#=====   Printing list to screen   =====#" << endl << endl << endl;
            if (itinerary)
            {
                inData.open(it);
                if (!inData)
                {
                    raiseFlag = true;
                    cout << "#== Cannot open file, exiting program ==#" << endl << endl;
                    return;
                }

                cout << "#*********************************#" << endl
                     << right << setw(19) << setfill('.') << name
                     << left << setw(16) << "'s Itinerary" << endl
                     << right << "#*********************************#" << endl;

                while (getline(inData, activity))
                    cout << activity << endl;
                inData.close();
            }

            cout << "#*********************************#" << endl;
            cout << setw(19) << setfill('.') << name
                 << left << "'s Camp Supplies" << endl
                 << showpoint << fixed << setprecision(2)
                 << setw(28) << "Size of camping party:" 
                 << right << setw(7) << campers << endl
                 << left << setw(28) << "Duration of stay:"
                 << right << setw(7) << nightsStaying << endl
                 << setfill('-') << "#" << setw(34) << "#" << endl
                 << "#" << setw(34) << "#" << endl
                 << setfill('.') << "#" << setw(16) << "Things "
                 << left << setw(17) << "to pack" << "#" << endl
                 << "Need: " << static_cast<int>((campers / 2.0) + .5)
                 << "x 2-man OR " << static_cast<int>((campers / 5.0) + .8)
                 << "x 5-man tent(s)" << endl
                 << setw(28) << "- - - Sleeping bags"
                 << right << setw(7) << campers << endl
                 << left << setw(28) << "- - - Fire starters"
                 << right << setw(7) << fireStarter << endl
                 << left << setw(28) << "- - - Lbs of Marshmallow"
                 << right << setw(7) << lbsMarshmallow << endl;

            if (ch == 'y' || ch == 'Y')
            {
                printItems(foodList, gearList, foodCount, gearCount, length);
                if (raiseFlag)
                    return;
            }

            cout << "#" << setfill('-') << setw(34) << "#" << endl
                 << setfill('.') << setw(26) << "And lots of camping "
                 << left << "spirit!!!" << endl
                 << "#*********************************#" << endl << endl << endl;
            break;

        case 3:
            outData.open(rpt);
            if (!outData)
            {
                raiseFlag = true;
                cout << "#== Cannot open report file for writing ==#" << endl << endl;
                return;
            }

            cout << "#======    Saving your report now...  ==#" << endl;

            if (itinerary)
            {
                inData.open(it);
                if (!inData)
                {
                    raiseFlag = true;
                    cout << "#== Cannot open itinerary file ==#" << endl << endl;
                    return;
                }

                outData << "#*********************************#" << endl
                       << setw(19) << setfill('.') << name
                       << left << setw(17) << "'s Itinerary" << endl
                       << right << "#*********************************#" << endl;

                while (getline(inData, activity))
                    outData << activity << endl;
                inData.close();
            }

            outData << "#*********************************#" << endl;
            outData << setw(19) << setfill('.') << name
                   << left << "'s Camp Supplies" << endl
                   << showpoint << fixed << setprecision(2)
                   << setw(28) << "Size of camping party:" 
                   << right << setw(7) << campers << endl
                   << left << setw(28) << "Duration of stay:"
                   << right << setw(7) << nightsStaying << endl
                   << setfill('-') << "#" << setw(34) << "#" << endl
                   << "#" << setw(34) << "#" << endl
                   << setfill('.') << "#" << setw(16) << "Things "
                   << left << setw(17) << "to pack" << "#" << endl
                   << "Need: " << static_cast<int>((campers / 2.0) + .5)
                   << "x 2-man OR " << static_cast<int>((campers / 5.0) + .8)
                   << "x 5-man tent(s)" << endl
                   << setw(28) << "- - - Sleeping bags"
                   << right << setw(7) << campers << endl
                   << left << setw(28) << "- - - Fire starters"
                   << right << setw(7) << fireStarter << endl
                   << left << setw(28) << "- - - Lbs of Marshmallow"
                   << right << setw(7) << lbsMarshmallow << endl;

            if (ch == 'y' || ch == 'Y')
            {
                addItems(foodList, gearList, foodCount, gearCount, length, outData, raiseFlag);
                if (raiseFlag)
                    return;
            }

            outData << "#" << setfill('-') << setw(34) << "#" << endl
                   << setfill('.') << setw(26) << "And lots of camping "
                   << left << "spirit!!!" << endl
                   << "#*********************************#" << endl << endl << endl;

            cout << "#========      Save complete!     ======#" << endl;
            break;

        default:
            cout << "#  Invalid menu selection - no output   #" << endl
                 << "# will be generated. Please restart the #" << endl
                 << "#============     program to try again. #" << endl;
            raiseFlag = true;
            return;
        }
    } while (menu == 1);
}

void SupplyList::itFunc(int nightsStaying, string name, bool& itinerary)
{
    ofstream outData;

    itinerary = true;
    outData.open(it);
    cout << endl << "# Let's build an itinerary for the trip #" << endl << endl;
    cin.clear();
    cin.ignore(1, '\n');
    for (int n = 1; n <= nightsStaying; n++)
    {
        string activity;
        cout << "#==  What activity do you have planned =#" << endl
            << "#================ = - -   for day " << n << "? ===#" << endl
            << "# (You can say things like \"Kayaking\", =#" << endl
            << "#=========   \"Hiking\", or \"Fishing\"  ===#" << endl;
        getline(cin, activity);
        cout << "<(" << activity << ") added to itinerary>" << endl << endl;
        outData << left << setfill(' ') << "*Day " << right << setw(2) << n
            << ": " << right << setw(25) << activity << "*" << endl;
    }
    outData.close();
    cout << "# Perfect! I've created your ===========#" << endl
        << "#=============  personalized itinerary. #" << endl;
}

void SupplyList::colorText()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COLOR);
}

