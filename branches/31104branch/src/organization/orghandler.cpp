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

#include <includes.h>
#include <externs.h>
 
orgHandler::orgHandler() : nextID(0)
{
}

// Return reference to the organization with the given ID
organization &orgHandler::getOrg(int ID)
{
   list<organization>::iterator iter; // Iterator to step through my organization list

	// Step through my list of organizations
   for(iter=gOrgs.begin(); iter!=gOrgs.end(); iter++)
   {
      // If this is the organization I'm looking for, return it
      if(iter->ID = ID)return *iter;
   }

   // Organization of that ID not in my list? Throw exception
   // This should never have to happen
   char error[80];
   sprintf(error,"orgHandler recieved invalid ID %d in getOrg; no such object.",ID);
   throw invalid_argument(error);
}

// Add an organization
void orgHandler::addOrg(organization org)
{
   list<organization>::iterator iter; // Iterator to step through my organization list

   // Assign an unused ID to the organization
   org.ID = nextID++;
   // Add it to the list
   gOrgs.push_back(org);

   // Now update all organizations' records
   // Step through my list of organizations
   for(iter=gOrgs.begin(); iter!=gOrgs.end(); iter++)
   {
      // If this organization is the new one, skip it
      if(iter->ID == org.ID) continue;
      // Else notify this organization and the new one of eachother's existance
      iter->addOrgRecord(gOrgs.back());
      gOrgs.back().addOrgRecord(*iter);
   }
}

// Delete an organization
void orgHandler::deleteOrg(organization org)
{
	list<organization>::iterator iter1; // Iterator to step through the organization list

   // Step through all organizations
	for(iter1 = gOrgs.begin(); iter1 != gOrgs.end(); iter1++)
	{
      // If this is the org to be deleted, we'll remove it from the list
		if(iter1->ID == org.ID)
		{
         // Remove it
			gOrgs.erase(iter1);
			// Then go back one, so we don't skip the next one.
			iter1--;
		}
      // Else delete this org's internal record of the org we're deleting
		else iter1->deleteOrgRecord(org.ID);
	}
}

void orgHandler::swayAll()
{
}

void orgHandler::swayOrg(int ID, int opinionOrgID, int power)
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
