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

#include <externs.h>

void mode_title()
{
   //title screen
   erase();

   char str[100];

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"Liberal Crime Squad");
   move(4,39-((len(str)-1)>>1));
   addstr(str);

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"Inspired by the 1983 version of Oubliette");
   move(6,39-((len(str)-1)>>1));
   addstr(str);

   if(loaded)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,39-((len(slogan)-1)>>1));
      addstr(slogan);

      move(9,10);

      addstr("Liberals Indoctrinated: ");
      addstr(stat_recruits);

      move(10,10);
      addstr("Liberals Martyred: ");
      addstr(stat_dead);

      move(11,10);
      addstr("Conservatives Killed: ");
      addstr(stat_kills);

      move(9,47);
      addstr("Funds Taxed: ");
      addstr(ledger.total_income);

      move(10,47);
      addstr("Funds Spent: ");
      addstr(ledger.total_expense);

      move(11,47);
      addstr("Conservatives Kidnapped: ");
      addstr(stat_kidnappings);
   }

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"v3.9 Copyright (C) 2002-4, Tarn Adams");
   move(13,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"A Bay 12 Games Production");
   move(14,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"www.bay12games.com");
   move(15,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"v" PACKAGE_VERSION " Maintained by the Open Source Community");
   move(17,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"sourceforge.net/projects/lcsgame");
   move(18,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press ESC now to quit.  Quitting later causes your progress to be saved.");
   move(20,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press any other key to pursue your Liberal Agenda!");
   move(22,39-((len(str)-1)>>1));
   addstr(str);
   move(24,79);
   addstr("+");

   if(getkey()==ESC) end_game();

   viewhighscores();

   if(!loaded)
   {
      setup_newgame();
      makecharacter();
   }
   mode=GAMEMODE_BASE;
   mode_base();

   savegame("save.dat");
}
