/*

Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
                                                                                      //
This file is part of Liberal Crime Squad.                                             //
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

class interOrgData
{
public:
	interOrgData(int ID, float swayLevel, float affiliation, int respectLevel, int allyLevel);

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

class organization
{
public:
	organization(int newID);
	int attitude[VIEWNUM];
	vector<interOrgData> orgs;
	vector<int> specialInterests;

	//this says if they care about stuff other then special interests
	bool swayable;

	//These handle changing other organizations viewpoints
	void swayOthers();
	void attackedHandler();
	void calcAllyLevel(int ID);

	short attackPower;
	//Soldiers they will send to attack
	int soldiers[5];
private:
	int ID;
};

#endif