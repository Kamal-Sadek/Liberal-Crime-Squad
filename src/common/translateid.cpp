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
        This file was created by Chris Johnson (grundee@users.sourceforge.net)
        by copying code from game.cpp.
        To see descriptions of files and functions, see the list at
        the bottom of includes.h in the top src folder.
*/

#include <includes.h>
#include <externs.h>

/* transforms a squad id number into the index of that squad in the global vector */
int getsquad(int id)
{
   for(int i=0;i<len(squad);i++) if(squad[i]->id==id) return i;
   return -1;
}

/* transforms a car id number into the index of that car in the global vector */
int id_getcar(int id)
{
   for(int i=0;i<len(vehicle);i++) if(vehicle[i]->id()==id) return i;
   return -1;
}

/* transforms a creature id number into the index of that person in the pool */
int getpoolcreature(int id)
{
   for(int i=0;i<len(pool);i++) if(pool[i]->id==id) return i;
   return -1;
}

/* transforms a vehicle type id into the index of that vehicle type in the global vector */
int getvehicletype(int id)
{
   for(int i=0;i<len(vehicletype);i++) if(vehicletype[i]->id()==id) return i;
   return -1;
}

/* transforms a vehicle type idname into the index of that vehicle type in the global vector */
int getvehicletype(const string &idname)
{
   for(int i=0;i<len(vehicletype);i++) if(vehicletype[i]->idname()==idname) return i;
   return -1;
}

/* transforms a clip type id into the index of that clip type in the global vector */
int getcliptype(int id)
{
   for(int i=0;i<len(cliptype);i++) if(cliptype[i]->get_id()==id) return i;
   return -1;
}

/* transforms a clip type name into the index of that clip type in the global vector */
int getcliptype(const string &idname)
{
   for(int i=0;i<len(cliptype);i++) if(cliptype[i]->get_idname()==idname) return i;
   return -1;
}

/* transforms a weapon type id into the index of that weapon type in the global vector */
int getweapontype(int id)
{
   for(int i=0;i<len(weapontype);i++) if(weapontype[i]->get_id()==id) return i;
   return -1;
}

/* transforms a weapon type name into the index of that weapon type in the global vector */
int getweapontype(const string &idname)
{
   for(int i=0;i<len(weapontype);i++) if(weapontype[i]->get_idname()==idname) return i;
   return -1;
}

/* transforms a armor type id into the index of that armor type in the global vector */
int getarmortype(int id)
{
   for(int i=0;i<len(armortype);i++) if(armortype[i]->get_id()==id) return i;
   return -1;
}

/* transforms a armor type name into the index of that armor type in the global vector */
int getarmortype(const string &idname)
{
   for(int i=0;i<len(armortype);i++) if(armortype[i]->get_idname()==idname) return i;
   return -1;
}

/* transforms a loot type id into the index of that loot type in the global vector */
int getloottype(int id)
{
   for(int i=0;i<len(loottype);i++) if(loottype[i]->get_id()==id) return i;
   return -1;
}

/* transforms a loot type name into the index of that loot type in the global vector */
int getloottype(const string &idname)
{
   for(int i=0;i<len(loottype);i++) if(loottype[i]->get_idname()==idname) return i;
   return -1;
}

/* transforms a CreatureTypes value into a pointer to that creature type */
const CreatureType* getcreaturetype(short crtype)
{
   for(int i=0;i<len(creaturetype);i++) if(crtype==creaturetype[i]->get_type()) return creaturetype[i];
   return NULL;
}

/* transforms a creature type name into a pointer to that creature type  */
const CreatureType* getcreaturetype(const std::string& crtype)
{
   for(int i=0;i<len(creaturetype);i++) if(crtype==creaturetype[i]->get_idname()) return creaturetype[i];
   return NULL;
}
