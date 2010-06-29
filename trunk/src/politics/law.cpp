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

#include "politics/law.h"
#include "politics/alignment.h"
#include <includes.h>

// Default constructor
law::law()
{
   return;
}

// Adds the lawtext to the screen
void law::addlawtext(bool gameover)
{
   // Set the appropriate color, then add the appropriate text
   if(gameover)
   {
      set_alignment_color(ALIGN_ARCHCONSERVATIVE,true);
      addstr(defeat_lawtext.c_str());
   }
   else
   {
      set_alignment_color(alignment,true);
      addstr(lawtext[alignment+2].c_str()); // Alignment starts at -2, array starts at 0
   }
}

// Set law's alignment
void law::changealignment(signed char align)
{
   alignment = align;
}

// Set law's text for an alignment
void law::setlawtext(signed char align,string text)
{
   if(align==-3) // Defeat
      defeat_lawtext = text;
   else
      lawtext[align+2] = text;
}
