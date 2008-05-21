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

void mode_title(void)
{
   //title screen
   erase();

   char str[100];

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"Liberal Crime Squad");
   move(4,39-((strlen(str)-1)>>1));
   addstr(str);

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"Inspired by the 1983 version of Oubliette");
   move(6,39-((strlen(str)-1)>>1));
   addstr(str);

   if(loaded)
   {
      char num[20];

      int l=strlen(slogan);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,40-(l>>1));
      addstr(slogan);

      move(9,10);

      addstr("Liberals Indoctrinated: ");
      itoa(stat_recruits,num,10);
      addstr(num);

      move(10,10);
      addstr("Liberals Martyred: ");
      itoa(stat_dead,num,10);
      addstr(num);

      move(11,10);
      addstr("Conservatives Killed: ");
      itoa(stat_kills,num,10);
      addstr(num);

      move(9,47);
      addstr("Funds Taxed: ");
      itoa(stat_funds,num,10);
      addstr(num);

      move(10,47);
      addstr("Funds Spent: ");
      itoa(stat_spent,num,10);
      addstr(num);

      move(11,47);
      addstr("Conservatives Kidnapped: ");
      itoa(stat_kidnappings,num,10);
      addstr(num);
   }

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"v3.9 Copyright (C) 2002-4, Tarn Adams");
   move(13,39-((strlen(str)-1)>>1));
   addstr(str);
   strcpy(str,"A Bay 12 Games Production");
   move(14,39-((strlen(str)-1)>>1));
   addstr(str);
   strcpy(str,"www.bay12games.com");
   move(15,39-((strlen(str)-1)>>1));
   addstr(str);
   strcpy(str,"v" PACKAGE_VERSION " Maintained by the Open Source Community");
   move(17,39-((strlen(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press ESC now to quit.  Quitting later causes your progress to be saved.");
   move(20,39-((strlen(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press any other key to pursue your Liberal Agenda!");
   move(22,39-((strlen(str)-1)>>1));
   addstr(str);
   move(24,79);
   addstr("+");

   int c=getch();
   translategetch(c);
   if(c==27)
   {
      end_game();
   }

   viewhighscores();

   if(!loaded)makecharacter();
   mode=GAMEMODE_BASE;
   mode_base();

   save();
}


void loadinitfile(void)
{
   //LOAD OPTIONS IF POSSIBLE
   ::fstream fseed;
   #ifdef WIN32_PRE_DOTNET
   //This uses some non-standard isotream stuff which Microsoft have got
   //rid of in the .NET 2003 version of their C++ library (the 3rd arg and nocreate)
   fseed.open("init.txt",ios::nocreate | ios::in,filebuf::sh_read);
   #else
   fseed.open("init.txt",ios::in);
   #endif
   if(fseed.is_open())
   {
      int count=0;
      char str[200];
      char word[3][205];
      int wordnum,pos;
      char begin;
      while(fseed.getline(str,198)&&count<10000)
      {
         if(str[0]!='#')
         {
            wordnum=0;pos=0;begin=1;
            for(int l=0;l<strlen(str);l++)
            {
               if(str[l]==' '||str[l]=='\t')
               {
                  if(str[l+1]!=' '&&str[l+1]!='\t'&&!begin)
                  {
                     word[wordnum][pos]='\x0';
                     wordnum++;
                     pos=0;
                  }
                  continue;
               }
               if(wordnum>2)break;
               word[wordnum][pos]=str[l];
               pos++;
               begin=0;
            }
            if(wordnum<=2)word[wordnum][pos]='\x0';

            if(!stricmp(word[1],"="))
            {
               if(!stricmp(word[0],"pagekeys"))
               {
                  if(!stricmp(word[2],"AZERTY"))
                  {
                     interface_pgup='.';
                     interface_pgdn='/';
                  }
                  else if(!stricmp(word[2],"brackets"))
                  {
                     interface_pgup='[';
                     interface_pgdn=']';
                  }
                  else if(!stricmp(word[2],"page"))
                  {
                     interface_pgup=-61;
                     interface_pgdn=-55;
                  }
               }
            }
         }
         count++;
         
      }
   }
   fseed.close();
}

