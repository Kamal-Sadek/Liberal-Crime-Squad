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

#include <includes.h>
#include <list>
#include <stdexcept>
#include <string>
#include "configfile.h"
#include "manager/manager.h"

class interOrgData : public managedObject
{
public:
	interOrgData();
	interOrgData(int ID, int respectLevel, int allyLevel);

	//this is how much you respect them, and how much they are your ally
	//allyLevel can go negative, respect level cannot.
	//heat is how much short-term hate there is.
	int respectLevel;
	int allyLevel;
	int heat;
};

class organization : public managedObject
{
public:
	vector<interOrgData> orgs;
	vector<int> specialInterests;

	organization();
	organization(int newID);
	organization(string newName);
	organization(int newID, string newName);

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
	
};

#endif