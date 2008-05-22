#include <externs.h>
#include "organization/testdriver.h"



void testdriver()
{
	//The org Mom creates the organizations.
	organization *orgMom;

	organization conservative("conservative");
	conservative.alignment = ALIGN_CONSERVATIVE;
	conservative.attackPower = 9001; //It's over 9000!!!!!!!!!!!!
	organization liberal("liberal");
	liberal.alignment = ALIGN_LIBERAL;
	liberal.attackPower = 10;

	for(int i = 0; i < VIEWNUM; i++)
	{
		liberal.specialInterests.push_back((enum Views)i);
		conservative.specialInterests.push_back((enum Views)i);
	}
	gOrgHandler.addOrg(liberal);
	gOrgHandler.addOrg(conservative);
	string newName;
	for(int j = 0; j < 10; j++)
	{
		newName = "Test";
		newName += (j+'0');
		orgMom = new organization(newName);
		if(LCSrandom(2))
		{
			orgMom->alignment = ALIGN_CONSERVATIVE;
		}
		else
		{
			orgMom->alignment = ALIGN_LIBERAL;
		}
		for(int k = 0; k < 4; k++)
		{
			orgMom->specialInterests.push_back((enum Views)LCSrandom(VIEWNUM));
		}
		orgMom->attackPower = LCSrandom(20);
		gOrgHandler.addOrg(*orgMom);
	}
	
	move(0,0);
	addstr("Test Driver Completed");
	refresh();
	getch();

	//gOrgHandler.addOrg();
}