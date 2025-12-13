#include "SupplyList.h"

using namespace std;

int main()
{
    SupplyList sl;

    // user / result variables
    string name;
    char ch = '\0';
    int campers = 0;
    int nightsStaying = 0;
    int firesPlanned = 0;
    int fireStarter = 0;
    double lbsMarshmallow = 0.0;
    bool raiseFlag = false;

    // arrays for extra items (owned by main and passed into SupplyList functions)
    SupplyList::inventoryItem foodList[SupplyList::MAX_ARRAY];
    SupplyList::inventoryItem gearList[SupplyList::MAX_ARRAY];
    int foodCount = 0;
    int gearCount = 0;

	//colorText(); //option to change text color in console window

	bannerAndInput(name, campers, nightsStaying, firesPlanned, ch);
	extrasFunc(ch, foodList, gearList, foodCount, gearCount, MAX_ARRAY);
	calculations(campers, nightsStaying, firesPlanned, fireStarter,
		lbsMarshmallow);
	//void menuSelect(int& menu);
	printSave(foodCount, gearCount, foodList, gearList, MAX_ARRAY, name, campers, nightsStaying, 
		firesPlanned, fireStarter, lbsMarshmallow, ch, raiseFlag);

    if (raiseFlag)
    {
        return 1;
    }

    cout << "# Thanks for allowing me to assist you  #" << endl
         << "# with planning! I hope you found this  #" << endl
         << "# tool to be helpful - enjoy your trip! #" << endl << endl;

    return 0;
}

