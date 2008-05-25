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

interOrgData::interOrgData() : respectLevel(0), allyLevel(0) 
{ 
}

interOrgData::interOrgData(int IDin, int respectLevelin, int allyLevelin)
{
	ID = IDin;
	respectLevel = respectLevelin;
	allyLevel = allyLevelin;
}

organization::organization()
{
}


organization::organization(int newID)
{
	ID = newID;
}

organization::organization(string newName)
{
	name = newName;
}

organization::organization(int newID, string newName)
{
	ID = newID;
	name = newName;
}

interOrgData &organization::getOrgByID(int ID)
{
	vector<interOrgData>::iterator iter; // Iterator to step through my object list

	// Step through my list of objects
   for(iter=orgs.begin(); iter!=orgs.end(); iter++)
   {
      // If this is the object I'm looking for, return it
      if(iter->ID == ID)
	  {
		  return *iter;
	  }
   }

      // Object of that ID not in my list? Throw exception
   // This should never have to happen
   char error[80];
   sprintf(error,"manager recieved invalid ID %d in getObj; no such object.",ID);
   throw invalid_argument(error);
}

void organization::swayOthers()
{
	vector<int> candidates;
	for(int i = 0; i < orgs.size(); i++)
	{
		if(orgs[i].allyLevel > 30 || orgs[i].allyLevel < 20)
		{
			candidates.push_back(i);
		}
	}
	for(int i = 0; i < orgs.size(); i++)
	{
		if(orgs[i].allyLevel > 0)
		{
			for(int j = 0; j < candidates.size(); j++)
			{
				gOrgManager.swayOrg(ID, orgs[candidates[j]].ID, orgs[candidates[j]].allyLevel);
			}
		}
	}
}

void organization::attackedHandler()
{
}

int organization::spawnUnit(std::vector<int> unitList)
{
	int total = 0;
	std::vector<int> chance;
	chance.reserve(unitList.size());
	for(int i = 0; i < unitList.size(); i++)
	{
		chance.push_back(getSpawnChance((enum CreatureType)unitList.at(i)));
		total += chance.at(i);
	}
	int rnd;
	rnd = LCSrandom(total);
	for(int i = 0; i < unitList.size(); i++)
	{
		if(chance.at(i) > rnd)
		{
			return unitList.at(i);
		}
	}
	return unitList.at(0);
}

// Deletes the record of an organization
void organization::deleteOrgRecord(int deleteID)
{
   vector<interOrgData>::iterator iter; // Iterator to step through interOrgData vector

   // Step through my organization records
   for(iter=orgs.begin();iter!=orgs.end();iter++)
   {
      // Find the organization to delete
      if(iter->ID==deleteID)
      {
         // Delete it and stop looking
         orgs.erase(iter);
         return;
      }
   }
}

// Adds record of an organization if it has common special interests
void organization::addOrgRecord(const organization& org)
{
   // Check for and count common special interests
   vector<int>::const_iterator iter1; // Iterator to step through my interests
   vector<int>::const_iterator iter2; // Iterator to step through their interests

   int matchNum = 0;  // Number of common special interests

   // Loop through my special interests
	for(iter1=specialInterests.begin(); iter1!=specialInterests.end(); iter1++)
	{
		// Loop through their special interests
      for(iter2=org.specialInterests.begin();iter2!=org.specialInterests.end();iter2++)
		{
         // If we share this interest
			if(*iter1 == *iter2)
			{
				matchNum++; // Count this as a match
				break;      // Continue to next interest pair
			}
		}
	}

   // If we don't share any interests, I don't care about their
   // organization and will decline to create a record of them
   if(matchNum == 0) return;

   // Determine whether we are allies or enemies on our shared
   // interests by comparing alignments
   char allied;
	if(alignment == org.alignment)
		allied = +1; // We are allies
	else
		allied = -1; // We are enemies

   interOrgData newdata; // Data record to add
   
   // Record their data
   newdata.ID           = org.ID;
   // The more common issues we have, the closer we are,
   // or the more fierce our rivalry
   newdata.allyLevel    = 20 * matchNum * allied;

   newdata.heat = 0;

   // Add this record to our list
   orgs.push_back(newdata);
}
