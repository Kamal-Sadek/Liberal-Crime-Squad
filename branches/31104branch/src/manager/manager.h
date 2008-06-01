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
#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

//#include "configfile.h"
#include <string>
#include "serializer.h"

class managedObject : public serializer
{
public:
	int ID;
	std::string type;
};

template <class T> class manager : public serializer
{
public:
	//This handles all entitys
	manager();

   // Return reference to the object with the given ID
	T &getObj(int ID);

	//Returns reference to any objects with the given type
	std::vector<int> getIDByType(std::string type);

   // Adds a new object to the list, assigning a free ID
	void addObj(T obj);
   // Deletes an object from the list
	void deleteObj(T obj);

	int getSize()
	{
		return objects.size();
	}

	int getIDByIndex(int index)
	{
		return objects.at(index).ID;
	}

	T &getObjByIndex(int index)
	{
		return objects.at(index);
	}

	void saveLoadHandler(std::fstream *stream, bool reading)
	{
		serializeVectorHandler<T>(stream, reading, objects);
		serializeHandler(stream, reading, nextID);
	}

protected:
	std::vector<T> objects; // Internal object list
    int nextID;
};
 
template <class T> manager<T>::manager() : nextID(0)
{
}

// Return reference to the object with the given ID
template <class T> T &manager<T>::getObj(int ID)
{
   vector<T>::iterator iter; // Iterator to step through my object list

	// Step through my list of objects
   for(iter=objects.begin(); iter!=objects.end(); iter++)
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

// Return reference to the object with the given ID
template <class T> std::vector<int> manager<T>::getIDByType(std::string type)
{
   vector<T>::iterator iter; // Iterator to step through my object list
   vector<int> matchingObjects;

	// Step through my list of objects
   for(iter=objects.begin(); iter!=objects.end(); iter++)
   {
      // If this is the object I'm looking for, return it
      if(iter->type == type)
	  {
		  matchingObjects.push_back((*iter).ID);
	  }
   }
   return matchingObjects;
}

// Add an object
template <class T> void manager<T>::addObj(T obj)
{
   // Assign an unused ID to the object
   obj.ID = nextID++;
   // Add it to the list
   objects.push_back(obj);
}

// Delete an object
template <class T> void manager<T>::deleteObj(T obj)
{
	vector<object>::iterator iter; // Iterator to step through the object list

   // Step through all objects
	for(iter = objects.begin(); iter != objects.end(); iter++)
	{
      // If this is the org to be deleted, we'll remove it from the list
		if(iter->ID == obj.ID)
		{
         // Remove it
			objects.erase(iter1);
			// Then go back one, so we don't skip the next one.
			iter--;
		}
      // Else delete this org's internal record of the org we're deleting
		else iter1->deleteOrgRecord(obj.ID);
	}
}



//----------------------------------------------------------------------------------


template <class T, class copyClass> class defManager : public manager<T>
{
public:
	defManager(std::string inType)
	{
		nextID = 0;
		type = inType;
	}

	copyClass* getInstance(int baseID) 
	{ 
		copyClass* instance = new copyClass; 
		initialize(*instance, baseID); 
		return instance; 
	}

	void initialize(copyClass &instance, int ID) 
	{ 
		/* find the correct object */ 
		getObj(ID).initializeInstance(instance); 
	}
	std::string type;
};





#endif