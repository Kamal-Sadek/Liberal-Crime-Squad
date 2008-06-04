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

#ifndef ORGANIZATION_H_INCLUDED
#define ORGANIZATION_H_INCLUDED

//#include "manager/manager.h"

class interOrgData : public managedObject
{
public:
	interOrgData();
	interOrgData(int ID, float respectLevel, float allyLevel);

	void saveLoadHandler(std::fstream *stream, bool reading);

	//this is how much you respect them, and how much they are your ally
	//allyLevel can go negative, respect level cannot.
	//heat is how much short-term hate there is.
	float respectLevel;
	float allyLevel;
	int heat;
};

class orgAIGoal : public managedObject
{
public:
	void saveLoadHandler(std::fstream *stream, bool reading);

	//goals base power
	int basePower;
	//the multiplier
	int powerMult;

	int targetID;
	int priority;
};

class organization : public managedObject
{
public:

	organization();
	organization(int newID);
	organization(std::string newName);
	organization(int newID, std::string newName);

	interOrgData &getOrgByID(int ID);
	
	//These handle changing other organizations viewpoints
	void swayOthers();
	void attackedHandler();
	void calcAllyLevel(int ID);

	int spawnUnit(std::vector<int> unitList);

    // Deletes the record of an organization
    void deleteOrgRecord(int deleteID);
    // Adds record of an organization if it has common special interests
    void addOrgRecord(const organization& org);

	//runs ze org AI
	void orgAI();
	void activateGoal();
	void generateGoal(std::string inType, int inTargetID);

	//Saving and loading handler
	void saveLoadHandler(std::fstream *stream, bool reading);

	char alignment;

	//attack information
	//how good they are at attacking
	short attackPower;
	std::vector<int> soldiers;

	//Lobby information
	//how good they are at convincing other organizations
	short lobbyPower;
	std::vector<int> lobbyists;

	//Publicity information
	//how good they are at convincing the public
	short publicityPower;
	std::vector<int> publicists;

	std::string name;

	std::vector<interOrgData> orgs;
	std::vector<int> specialInterests;
	manager<orgAIGoal> goals;
	int currGoal;
};

#endif