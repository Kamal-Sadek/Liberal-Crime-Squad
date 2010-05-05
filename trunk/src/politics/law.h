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


#ifndef LAW_H_INCLUDED
#define LAW_H_INCLUDED

#include <string>

class law
{
private:
   std::string lawtext[5];      // Describes the law for each alignment
   std::string defeat_lawtext;  // Describes the law when the game is lost

   signed char alignment;
public:
   law();        // Default constructor
   
   void addlawtext(bool gameover=false); // Adds the lawtext to the screen
   void changealignment(signed char align);

   void setlawtext(signed char align,std::string text);
};

#endif //LAW_H_INCLUDED
