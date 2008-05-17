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
#ifndef ORGHANDLER_H_INCLUDED
#define ORGHANDLER_H_INCLUDED
#include "organization.h"

class orgHandler
{
public:
	//This handles all organizations
	orgHandler();

   // Return reference to the organization with the given ID
	organization &getOrg(int ID);

   // Adds a new organization to the list, assigning a free ID
   // and notifying all existing organizations of its appearance
	void addOrg(organization org);
   // Deletes an organization from the list, notifying all
   // existing organizations of its deletion
	void deleteOrg(organization org);
	
	//This makes all organizations sway each other.
	void swayAll();

	void swayOrg(int ID, int opinionOrgID, int power);

private:
   list<organization> gOrgs; // Internal organization list
};
#endif