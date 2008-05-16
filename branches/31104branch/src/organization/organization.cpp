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

#include <includes.h>
#include <externs.h>
#include "organization.h"



organization::organization(int newID)
{
	ID = newID;
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
				//gOrgHandler.swayOrg(ID, orgs[candidates[j]].ID, orgs[candidates[j]].allyLevel);
			}
		}
	}
}

void organization::attackedHandler()
{
}
