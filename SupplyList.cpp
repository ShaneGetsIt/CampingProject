#include "SupplyList.h"

// Prompt user to add extra food/gear items and store them in the lists
void SupplyList::extrasFunc(char ch, int length)
{
    int choice = 0;

    if (ch == 'y' || ch == 'Y') {

        cin.ignore(1, '/n');
        cout << "# You may add up to 5 food items and/or #" << endl
            << "#===== - - - -            5 gear items =#" << endl << endl;

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

                // Add food entry if room
                if (foodCount < length)
                {
                    foodList[foodCount].item = food;

                    cin.ignore(50, '\n');
                    cout << "#= Enter food item name to add to list =#" << endl << endl;
                    getline(cin, foodList[foodCount].itemName);
                    cout << "# Units of " << left << setfill('.') << setw(30) << foodList[foodCount].itemName
                        + " to bring " << "#" << endl;
                    cin >> foodList[foodCount].quantity;
                    cout << "# How important is this item on a scale #" << endl
                        << "#= of 1 (low), 2 (medium), or 3 (high)? #" << endl << endl;
                    cin >> choice;
                    while (!choice || choice < 1 || choice > 3)
                    {
                        cout << " Invalid selection - must enter int's 1-3" << endl;
                        cin.clear();
                        cin.ignore(12, '\n');
                        cin >> choice;
                    }
                    foodList[foodCount].prio = static_cast<priority>(choice - 1);
                    foodCount++;

                }
                else
                    cout << " Limit for additional food items reached" << endl << endl;
                break;

            case gear:

                // Add gear entry if room
                if (gearCount < length)
                {
                    gearList[gearCount].item = gear;
                    cin.ignore(50, '\n');
                    cout << "#= Enter gear item name to add to list =#" << endl
                        << "# for example: kayak, fishing rod, etc. #" << endl << endl;
                    getline(cin, gearList[gearCount].itemName);
                    cout << "# Units of " << left << setfill('.') << setw(30) << gearList[gearCount].itemName
                        + " to bring " << "#" << endl;
                    cin >> gearList[gearCount].quantity;
                    cout << "# How important is this item on a scale #" << endl
                        << "#= of 1 (low), 2 (medium), or 3 (high)? #" << endl << endl;
                    cin >> choice;
                    while (!choice || choice < 1 || choice > 3)
                    {
                        cout << " Invalid selection - must enter int's 1-3" << endl;
                        cin.clear();
                        cin.ignore(12, '\n');
                        cin >> choice;
                    }
                    gearList[gearCount].prio = static_cast<priority>(choice - 1);
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

// Write stored extra items to the provided output stream
void SupplyList::addItems(ofstream& outData, bool& raiseFlag, int length)
{
    int i;

    if (!outData)
    {
        cout << "#=====   Output file is not open   =====#" << endl << endl;
        raiseFlag = true;
        return;
    }

    if (foodCount != 0)
    {
        outData << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra food to pack" << "Priority" << endl;

        for (i = 0; i < foodCount; i++)
        {
            outData << left << foodList[i].quantity << setw(30) << "x " + foodList[i].itemName;
            if (foodList[i].prio == 0)
            {
                outData << right << setw(4) << "Low" << endl;
            }
            else if (foodList[i].prio == 1)
            {
                outData << right << setw(4) << "Med" << endl;
            }
            else if (foodList[i].prio == 2)
            {
                outData << right << setw(4) << "High" << endl;
            }
        }
    }

    if (gearCount != 0)
    {
        outData << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra gear to pack" << "Priority" << endl;

        for (i = 0; i < gearCount; i++)
        {
            outData << left << gearList[i].quantity << setw(30) << "x " + gearList[i].itemName;
            if (gearList[i].prio == 0)
            {
                outData << right << setw(4) << "Low" << endl;
            }
            else if (gearList[i].prio == 1)
            {
                outData << right << setw(4) << "Med" << endl;
            }
            else if (gearList[i].prio == 2)
            {
                outData << right << setw(4) << "High" << endl;
            }
        }
    }
}

// Print stored extra items to console
void SupplyList::printItems(int length)
{
    int i;

    if (foodCount != 0)
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra food to pack" << "Priority" << endl;

        for (i = 0; i < foodCount; i++)
        {
            cout << left << foodList[i].quantity << setw(30) << "x " + foodList[i].itemName;
            if (foodList[i].prio == 0)
            {
                cout << right << setw(4) << "Low" << endl;
            }
            else if (foodList[i].prio == 1)
            {
                cout << right << setw(4) << "Med" << endl;
            }
            else if (foodList[i].prio == 2)
            {
                cout << right << setw(4) << "High" << endl;
            }
        }
    }

    if (gearCount != 0)
    {
        cout << right << setw(35) << setfill('-') << "-" << endl
            << setw(27) << setfill('.') << left << "Extra gear to pack" << "Priority" << endl;

        for (i = 0; i < gearCount; i++)
        {
            cout << left << gearList[i].quantity << setw(30) << "x " + gearList[i].itemName;
            if (gearList[i].prio == 0)
            {
                cout << right << setw(4) << "Low" << endl;
            }
            else if (gearList[i].prio == 1)
            {
                cout << right << setw(4) << "Med" << endl;
            }
            else if (gearList[i].prio == 2)
            {
                cout << right << setw(4) << "High" << endl;
            }
        }
    }
}

