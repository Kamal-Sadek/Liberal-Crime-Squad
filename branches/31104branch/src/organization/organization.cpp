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
#include <stdexcept>
#include <string>
#include "configfile.h"



interOrgData::interOrgData() : respectLevel(0), allyLevel(0) 
{ 
}

interOrgData::interOrgData(int IDin, float respectLevelin, float allyLevelin)
{
	ID = IDin;
	respectLevel = respectLevelin;
	allyLevel = allyLevelin;
}

organization::organization()
{
	currGoal = -1;
}


organization::organization(int newID)
{
	ID = newID;
	currGoal = -1;
}

organization::organization(string newName)
{
	name = newName;
	currGoal = -1;
}

organization::organization(int newID, string newName)
{
	ID = newID;
	name = newName;
	currGoal = -1;
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
   //Also add goals while doing this
	if(alignment == org.alignment)
	{
		allied = +1; // We are allies
		generateGoal("LOBBY", org.ID);
	}
	else
	{
		allied = -1; // We are enemies
		generateGoal("ATTACK", org.ID);
	}

   interOrgData newdata; // Data record to add
   
   // Record their data
   newdata.ID           = org.ID;
   // The more common issues we have, the closer we are,
   // or the more fierce our rivalry
   //calculated by percentage of our matching issues and the percentage averaged then divided by 2, for a total of /4.
   newdata.allyLevel    = float((((float)matchNum / (float)org.specialInterests.size()) + 
								((float)matchNum / (float)specialInterests.size())) / 4 * allied);

   newdata.heat = 0;

   // Add this record to our list
   orgs.push_back(newdata);
}

void organization::orgAI()
{
   // If this organization isn't doing anything right now, then spend this turn deciding
   // what its next objective is
	if(currGoal == -1)
	{
		int newGoal = -1;
		int newGoalPriority = 0;
		int size = goals.getSize();
		for(int i = 0; i < size; i++)
		{
			//should this go here?  I don't think so, but whatever
         // (For publicity goals, determine priority level of the goal)
			if(goals.getObjByIndex(i).type == "PUBLICITY")
			{
				//This needs to be changed so organizations can be attacked with publicity
				if(alignment == -1)
				{
					goals.getObjByIndex(i).priority = (int)attitude[goals.getObjByIndex(i).targetID];
				}
				else if(alignment == 1)
				{
					goals.getObjByIndex(i).priority = (int)(100 - attitude[goals.getObjByIndex(i).targetID]);
				}
			}
			//again, I don't think this should go here, but whatever
         // (For attack goals, determine priority level of the goal)
			if(goals.getObjByIndex(i).type == "ATTACK")
			{
				for(int j = 0; j < orgs.size(); j++)
				{
					if(goals.getObjByIndex(i).targetID == orgs[j].ID)
					{
                  // Goal priority level is the amount of heat toward this organization
						goals.getObjByIndex(i).priority = orgs[j].heat;
						break;
					}
				}
			}
         // Use the goal with the highest priority so far as newGoal
			if(goals.getObjByIndex(i).priority > newGoalPriority && goals.getObjByIndex(i).basePower != 0)
			{
				newGoal = goals.getObjByIndex(i).ID;
				newGoalPriority = goals.getObjByIndex(i).priority;
			}
		}
      // Use the last newGoal (highest priority goal of all of them) as the new goal
		if(newGoal != -1)
		{
			currGoal = newGoal;
		}
	}
   // If organization is in the middle of acting on a goal, it will continue working on this
	else
	{
      // Reduce heat with all other organizations over time
		for(int i = 0; i < orgs.size(); i++)
		{
			if(orgs[i].heat > 0)
			{
				orgs[i].heat--;
			}
		}
      // Planning -- the longer the organization works on this goal,
      // the more the power multiplier increases on it as the
      // organization lays the groundwork for future actions
		goals.getObj(currGoal).powerMult++;

		// Once the power multiplier is high enough can act on the goal
      //This needs to be fleshed out, a LOT.
		if(goals.getObj(currGoal).powerMult >= 30)
		{
         // Chance of acting each day
			if(!LCSrandom(20))
			{
				activateGoal(); // Act
				currGoal = -1;  // Clear the goal so you can work on something else next turn
			}
		}
	}
}

void organization::activateGoal()
{
	//Plan is to eventually have a more automated system where you can set different powers for each org
	//like stealth, unity, etc.
	//for now these combersome if statements will have to do.
	if(goals.getObj(currGoal).type == "LOBBY")
	{
		//Not handling lobby right now.
		//Lobbying another organization will transfer any heat you have
		//to them, or lower heat, depending.
	}
	else if(goals.getObj(currGoal).type == "PUBLICITY")
	{
      // Organization conducts a publicity campaign to influence public opinion on a target issue
      // Right now this is done silently, it should generate a news article

      // Commented until a news article is added to communicate this behavior to the player

      /*
         // Increase public interest in the issue
		   public_interest[goals.getObj(currGoal).targetID] += //alignment *
            (goals.getObj(currGoal).powerMult * goals.getObj(currGoal).basePower) / 3000;
         // Change public opinion on the issue
		   attitude[goals.getObj(currGoal).targetID] +=
               alignment * (goals.getObj(currGoal).powerMult * goals.getObj(currGoal).basePower) / 3000;
      */
	}
	else if(goals.getObj(currGoal).type == "ATTACK")
	{
      // At the moment this just launches an attack against the Liberal Crime Squad
      
      // It should check to see what organization is to be attacked and only run this code if the
      // organization is the LCS, running alternate code (including probably a news article) for
      // other enemy organizations

      // Commented until organizations are more finished

      /*
		int hPop = 0;
		int hLoc = -1;
		int numpres = 0;
		//newGoal.basePower = attackPower;
		for(int l=0;l<location.size();l++)
		{
			if(!location[l]->siege.siege)
			{
				numpres = 0;
				for(int p=0;p<pool.size();p++)
				{
					if(!pool[p]->alive)continue; // Dead people don't count
					if(pool[p]->location!=l)continue; // People not at this base don't count
					if(pool[p]->align!=1)continue; // Non-liberals don't count other than that
					numpres++;
				}
				if(numpres > hPop)
				{
					hPop = numpres;
					hLoc = l;
				}
			}
		}
		if(hLoc != -1)
		{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			char gaspstring[40];
			sprintf(gaspstring, "The %s are raiding the ", name.c_str());
			addstr(gaspstring);
			addlocationname(location[hLoc]);
			addstr("!");

			refresh();
			getch();

			location[hLoc]->siege.siege=1;
			location[hLoc]->siege.orgID = ID;
			location[hLoc]->siege.siegetype=SIEGE_ORG;
			location[hLoc]->siege.underattack=1;
			location[hLoc]->siege.lights_off=0;
			location[hLoc]->siege.cameras_off=0;
			getOrgByID(gOrgManager.getOrgsByType("LCS").at(0)).heat = 0;
		}
      */
	}

   // Organization becomes satisfied that it took action on this,
   // so priority drops to zero
	goals.getObj(currGoal).priority = 0;
   // New plans must be laid out for it to act on this issue again,
   // so power multiplier drops to zero
	goals.getObj(currGoal).powerMult = 0;
}

void organization::generateGoal(std::string inType, int inTargetID)
{
	orgAIGoal newGoal;
	newGoal.targetID = inTargetID;
	newGoal.type = inType;

	//This if statements needs to disapear...
	if(inType == "LOBBY")
	{
		newGoal.basePower = lobbyPower;
	}
	else if(inType == "PUBLICITY")
	{
		newGoal.basePower = publicityPower;
	}
	else if(inType == "ATTACK")
	{
		newGoal.basePower = attackPower;
	}
	
	newGoal.powerMult = 0;
	newGoal.priority = 0;
	goals.addObj(newGoal);
}

void organization::saveLoadHandler(fstream *stream, bool reading)
{
	serializeHandler(stream, reading, alignment);

	serializeHandler(stream, reading, attackPower);
	serializeVectorHandler<int>(stream, reading, soldiers);

	serializeHandler(stream, reading, lobbyPower);
	serializeVectorHandler<int>(stream, reading, lobbyists);

	serializeHandler(stream, reading, publicityPower);
	serializeVectorHandler<int>(stream, reading, publicists);

	serializeHandler(stream, reading, name);

	serializeVectorHandler<interOrgData>(stream, reading, orgs);
	serializeVectorHandler<int>(stream, reading, specialInterests);
	goals.saveLoadHandler(stream, reading);
	serializeHandler(stream, reading, currGoal);

	serializeHandler(stream, reading, type);
	serializeHandler(stream, reading, ID);
}

void interOrgData::saveLoadHandler(fstream *stream, bool reading)
{
	serializeHandler(stream, reading, respectLevel);
	serializeHandler(stream, reading, allyLevel);
	serializeHandler(stream, reading, heat);
	serializeHandler(stream, reading, ID);
}

void orgAIGoal::saveLoadHandler(fstream *stream, bool reading)
{
	serializeHandler(stream, reading, basePower);
	serializeHandler(stream, reading, powerMult);
	serializeHandler(stream, reading, targetID);
	serializeHandler(stream, reading, priority);
	serializeHandler(stream, reading, type);
	serializeHandler(stream, reading, ID);
}