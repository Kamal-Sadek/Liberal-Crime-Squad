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
long getsquad(long id)
{
   for(int sq=0;sq<squad.size();sq++)
   {
      if(squad[sq]->id==id)return sq;
   }

   return -1;
}

/* transforms a car id number into the index of that car in the global vector */
long id_getcar(long id)
{
   for(long v=0;v<vehicle.size();v++)
   {
      if(vehicle[v]->id==id)return v;
   }
   return -1;
}

/* transforms a creature id number into the index of that person in the pool */
int getpoolcreature(long id)
{
   for(int pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->id==id)return pl;
   }
   return -1;
}
