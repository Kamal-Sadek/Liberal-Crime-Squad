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

#ifndef ORGANIZATIONDEF_H_INCLUDED
#define ORGANIZATIONDEF_H_INCLUDED

#include <includes.h>
#include <list>
#include <stdexcept>
#include <string>
#include "manager/manager.h"
#include "configfile.h"
#include "organization/organization.h"

class configInterOrgData : public configurable
{
public:
	void initConfig();

	//int ID;
	//These are percentages that you are willing to be swayed and are affiliated with this org
	float affiliation;
	float swayLevel;

	//this is how much you respect them, and how much they are your ally
	//difference between allyLevel and affiliation is, affiliation is more like partial ownership.
	//allyLevel can go negative, respect level cannot.
	int respectLevel;
	int allyLevel;
};

class organizationDef : public configurable
{
public:
	std::vector<int> specialInterests;

	void initConfig();
	void initializeInstance(organization &instance);

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