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

class interOrgData : public configurable
{
public:
	interOrgData();
	interOrgData(int ID, float swayLevel, float affiliation, int respectLevel, int allyLevel);
	void initConfig();

	int ID;
	//These are percentages that you are willing to be swayed and are affiliated with this org
	float affiliation;
	float swayLevel;

	//this is how much you respect them, and how much they are your ally
	//difference between allyLevel and affiliation is, affiliation is more like partial ownership.
	//allyLevel can go negative, respect level cannot.
	int respectLevel;
	int allyLevel;
};

class organization : public configurable
{
public:
	vector<interOrgData> orgs;
	vector<enum Views> specialInterests;

	organization();
	organization(int newID);
	organization(string newName);
	organization(int newID, string newName);

	void initConfig();

	//These handle changing other organizations viewpoints
	void swayOthers();
	void attackedHandler();
	void calcAllyLevel(int ID);

   // Deletes the record of an organization
   void deleteOrgRecord(int deleteID);
   // Adds record of an organization if it has common special interests
   void addOrgRecord(const organization& org);

   	//this says if they care about stuff other then special interests
	bool swayable;
	char alignment;

	short attackPower;
	//Soldiers they will send to attack
	enum CreatureType soldiers[5];
	int ID;
	string name;
	
};

#endif