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

// Note: this file is encoded in the PC-8 / Code Page 437 / OEM-US character set
// (The same character set used by Liberal Crime Squad when it is running)
// Certain special characters won't display correctly unless your text editor is
// set to use that character set, such as this e with an accent: ‚

// In Windows Notepad with the Terminal font, OEM/DOS encoding it should work fine.
// You can set this in Notepad by going to Format->Font and choosing the Terminal font,
// then choosing OEM/DOS in the Script dropdown box.

// In Notepad++ go to the Encoding menu, Character sets, Western European, OEM-US... easy!

// In Code::Blocks's editor go to Settings->Editor->the Other Settings tab and
// then pick WINDOWS-437 from the dropdown box and then choose the radio button
// to make this the default encoding and disable auto-detection of the encoding.
// Then close the file and reopen it (since Code::Blocks detects the encoding
// when it opens the file and it can't be changed after that; what we changed was
// how it detects encoding for files it opens in the future, not files already open).

// In Microsoft Visual C++, right-click the file in the Solution Explorer,
// select "Open With...", choose "C++ Source Code Editor (with encoding)",
// then choose "OEM United States - Codepage 437".

// In MS-DOS Editor (included with Windows as EDIT.COM in your system32 directory),
// the codepage will be correct already since it's running in a console window just
// like Liberal Crime Squad. Well OK, the encoding might be wrong, but then it's wrong
// in Liberal Crime Squad TOO, and to fix it, go to Control Panel, Regional and Language Settings,
// Advanced tab, and choose English (United States) from the dropdown box as the encoding
// for non-Unicode applications, then press OK.

// If you have a Linux or other UNIX-based system you are obviously smart enough
// to figure out for yourself how to open a file in OEM-US PC-8 codepage 437 in
// your favorite text editor. If you're on Mac OS X, well that's UNIX-based, figure
// it out for yourself.

#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
#include <includeEnum.h>
#include <includeCommon.h>

/*
stringconversion.cpp
*/
#include "common\\stringconversion.h"

/*
consolesupport.cpp
*/
#include "common\\consolesupport.h"

#include <includeNews.h>
//#include <includeFunctions.h>
//#include <includeTitle.h>

//#include <includeTalk.h>
#include <includeExternDefault.h>
//#include <includeExternPolitics.h>
extern short mode;
extern MusicClass music;

extern vector<vector<string>> real_quote;

char str[100];

void title() {
   //title screen
   erase();

   set_color(COLOR_GREEN,COLOR_BLACK,1);
   strcpy(str,"Liberal Crime Squad");
   move(2,39-((len(str)-1)>>1));
   addstr(str);

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"Inspired by the 1983 version of Oubliette");
   move(4,39-((len(str)-1)>>1));
   addstr(str);
   vector<string> quote = pickrandom(real_quote);
   strcpy(str, quote[0]);
   move(6, 39 - ((len(str) - 1) >> 1));
   addstr(str);
   strcpy(str, quote[1]);
   move(7, 39 - ((len(str) - 1) >> 1));
   addstr(str);
   strcpy(str, quote[2]);
   move(8, 39 - ((len(str) - 1) >> 1));
   addstr(str);
   strcpy(str, quote[3]);
   move(9, 39 - ((len(str) - 1) >> 1));
   addstr(str);

   strcpy(str,"v3.9 Copyright (C) 2002-4, Tarn Adams");
   move(11,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"A Bay 12 Games Production");
   move(12,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://bay12games.com/lcs/");
   move(13,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"v" PACKAGE_VERSION " Maintained by the Open Source Community");
   move(15,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"https://github.com/Kamal-Sadek/Liberal-Crime-Squad");
   move(16,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://sourceforge.net/projects/lcsgame/");
   move(17,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://www.bay12games.com/forum/index.php?board=3.0");
   move(18,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://lcs.wikidot.com/");
   move(19,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press ESC now to quit. Quitting later causes your progress to be saved.");
   move(21,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press any other key to pursue your Liberal Agenda!");
   move(23,39-((len(str)-1)>>1));
   addstr(str);
   move(24,79);
   addstr("+");
}

void choose_savefile_name()
{
   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   mvaddstr(0,0,"In what world will you pursue your Liberal Agenda?");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   mvaddstr(1,0,"Enter a name for the save file.");

   char savefile_temp[21];
   enter_name(2,0, savefile_temp, sizeof(savefile_temp)/sizeof(savefile_temp[0]), "");
   if(strcmp(savefile_temp, "") != 0) savefile_name = string(savefile_temp) + ".dat";
   else savefile_name = "save_" + to_string(LCSrandom(1000)) + ".dat";
}

void mode_title()
{
   title();

   int c=0;

   vector<string> savefiles;

   do {

      if(c=='h') {
         viewhighscores();
         getkey();
         title();
      }

      if(c=='m') music.enableIf(!music.isEnabled());
      if(music.isEnabled()) strcpy(str,"Press M to turn off the Music. Press H to view your Liberal High Score.");
      else strcpy(str,"Press M to turn on some Music. Press H to view your Liberal High Score.");
      move(22,39-((len(str)-1)>>1));
      addstr(str);
      if(c==ESC||c=='x') end_game();

      c=getkey();
   } while(c=='m'||c=='h'||c=='x'||c==ESC);

   savefiles = std::move(LCSSaveFiles());

   char loaded = savefiles.size() > 0;
   bool to_delete = false;

   if(!loaded)
   {
      choose_savefile_name();
      setup_newgame();
      makecharacter();
   } 
   else
   {      
      int p=0, y=2, page=0;

      while(true) 
      {
         erase();
         if(to_delete) 
         {
            set_color(COLOR_YELLOW,COLOR_RED,1);
            mvaddstr(0,0, "Delete a Save File");
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            mvaddstr(0,0,"Choose a Save File");
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(1,0,"ÄÄÄÄTitleÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
         for(p=page*19,y=2;p<savefiles.size()&&p<page*19+19;p++,y++)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0); //c==y+'a'-2);
            move(y,0);
            addchar(y+'A'-2);addstr(" - ");

			string &strtemp = savefiles[y-2];
            addstr(strtemp.substr(0, strtemp.find(".dat")));
         }

         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr("NEW GAME");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(22,0);
         if(!to_delete) addstr("Press a Letter to Select a Save File");
         else addstr("Press a Letter to Delete a Save File");
         addstr(", V to switch, or X to quit");
         move(23,0);
         addpagestr();

         c = getkey();

         //PAGE UP
         if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
         //PAGE DOWN
         if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<savefiles.size())page++;
         if(c>='a'&&c<='s')
         {
            int p=page*19+c-'a';
            if(!to_delete)
            {
               if(p<savefiles.size()) { savefile_name = savefiles[p]; break; }
               else if(p == savefiles.size())
               {
                  choose_savefile_name();
                  setup_newgame();
                  makecharacter();
                  break;
               }
            }
            else if(p<savefiles.size())
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               strcpy(str,"Are you sure you want to delete " + savefiles[p] + "? (y/n)");
               move(10,39-((len(str)-1)>>1));
               addstr(str);
               c=getkey();
               if(c=='y')
               {
                  LCSDeleteFile(savefiles[p].c_str(), LCSIO_PRE_HOME);
                  savefiles = std::move(LCSSaveFiles());
               }
               continue;
            }

         }
         else if(c=='v') to_delete = !to_delete;
         if(c==ESC||c=='x') end_game();
      }

      load(savefile_name);

   }
   mode=GAMEMODE_BASE;
   mode_base();

   savegame(savefile_name);
}
