/*
This file is part of Liberal Crime Squad.                                           //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/


/*
	This file was made by Brad (AKA Puzzlemaker)
*/

//#include <includes.h>
#include <externs.h>

 
orgManager::orgManager()
{
	nextID = 0;
}

// Return reference to the organization with the given ID
organization &orgManager::getOrg(int ID)
{
	return getObj(ID);
}

// Add an organization
void orgManager::addOrg(organization org)
{
   vector<organization>::iterator iter; // Iterator to step through my organization list


   //time to generate the orgs goals.

   for(int i = 0; i < org.specialInterests.size(); i++)
   {
	   //WARNING:  Currently cannot attack other organizations with publicity!
	   //this needs to be fixored!
	   org.generateGoal("PUBLICITY", org.specialInterests[i]);
   }


	addObj(org);

   // Now update all organizations' records
   // Step through my list of organizations
   for(iter=objects.begin(); iter!=objects.end(); iter++)
   {
      // If this organization is the new one, skip it
      if(iter->ID == org.ID) continue;
      // Else notify this organization and the new one of eachother's existance
      iter->addOrgRecord(objects.back());
      objects.back().addOrgRecord(*iter);
   }
}

// Delete an organization
void orgManager::deleteOrg(organization org)
{
	vector<organization>::iterator iter1; // Iterator to step through the organization list

   // Step through all organizations
	for(iter1 = objects.begin(); iter1 != objects.end(); iter1++)
	{
      // If this is the org to be deleted, we'll remove it from the list
		if(iter1->ID == org.ID)
		{
         // Remove it
			objects.erase(iter1);
			// Then go back one, so we don't skip the next one.
			iter1--;
		}
      // Else delete this org's internal record of the org we're deleting
		else iter1->deleteOrgRecord(org.ID);
	}
}

vector<int> orgManager::getOrgsByType(string type)
{
	return getIDByType(type);
}

void orgManager::swayAll()
{
}

void orgManager::swayOrg(int ID, int opinionOrgID, float power)
{
	/*organization &org = getOrg(ID);
	if(org.swayable)
	{
		for(int i = 0; i < org.orgs.size(); i++)
		{
			if(org.orgs[i].ID == opinionOrgID)
			{
				org.orgs[i].allyLevel += (int)(org.orgs[i].swayLevel * power);
				break;
			}
		}
	}*/
}


//Now for the big part:  THE ORGANIZATIONS AI!
void orgManager::runOrgAI()
{
	int size = objects.size();
	for(int i = 0; i < size; i++)
	{
		objects[i].orgAI();
	}
}