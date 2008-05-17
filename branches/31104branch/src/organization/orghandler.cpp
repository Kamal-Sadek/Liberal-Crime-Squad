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
 
orgHandler::orgHandler()
{
}

organization *orgHandler::getOrg(int ID)
{
	//Loop through, find the org, return it
	int i = 0;
	while(gOrgs[i].ID != ID && i < gOrgs.size())
	{
		i++;
	}
	if(i == gOrgs.size())
	{
		return NULL;
	}
	return &gOrgs[i];
}

void orgHandler::addOrg(organization org)
{
	//This is confusing.
	int size = gOrgs.size();
	int i;
	int k;
	int matchNum = 0;
	//Loop through the orgs
	for(i = 0; i < gOrgs.size(); i++)
	{
		//loop through the special interests of the org you are comparing to
		for(int j = 0; j < gOrgs[i].specialInterests.size(); j++)
		{
			int opposedSpecSize = org.specialInterests.size();
			//loop through the org you want to add, and compare to see if there are any matches
			for(k = 0; k < opposedSpecSize; k++);
			{
				if(org.specialInterests.at(k) == gOrgs.at(i).specialInterests.at(j))
				{
					matchNum++;
				}
			}
		}
		//if there are matches...
		if(matchNum > 0)
		{
			//Find if they are allied
			char allied;
			if(gOrgs[i].alignment == org.alignment)
			{
				allied = 1;
			}
			else
			{
				allied = -1;
			}
			//Init data
			interOrgData data(-1, 1.0f, 0.0f, 0, matchNum * 20 * allied);
			data.ID = org.ID;
			gOrgs[i].orgs.push_back(data);
			data.ID = gOrgs[i].ID;
			org.orgs.push_back(data);
			matchNum = 0;
		}
	}
	gOrgs.push_back(org);
}

void orgHandler::deleteOrg(organization org)
{
	//Now to delete an org
	vector<organization>::iterator iter1;
	vector<interOrgData>::iterator iter2;
	int i = 0;
	//this is confusing.
	for(iter1 = gOrgs.begin(); iter1 != gOrgs.end(); iter1++)
	{
		//If this is the org to be deleted, remove it
		if(gOrgs[i].ID == org.ID)
		{
			gOrgs.erase(iter1);
			//after removal, go back one, so we don't skip the next one.
			iter1--;
			i--;
		}
		else
		{
			int j = 0;
			for(iter2 = gOrgs[i].orgs.begin(); iter2 != gOrgs[i].orgs.end(); iter2++)
			{
				if(gOrgs[i].orgs[j].ID == org.ID)
				{
					gOrgs[i].orgs.erase(iter2);
					//DELETED
					iter2--;
					j--;
				}
				iter2++;
				j++;
			}
		}
		i++;
	}
}

void orgHandler::swayAll()
{
}

void orgHandler::swayOrg(int ID, int opinionOrgID, int power)
{
	organization *org;
	org = getOrg(ID);
	if(org->swayable)
	{
		for(int i = 0; i < org->orgs.size(); i++)
		{
			if(org->orgs[i].ID == opinionOrgID)
			{
				org->orgs[i].allyLevel += (int)(org->orgs[i].swayLevel * power);
				break;
			}
		}
	}
}